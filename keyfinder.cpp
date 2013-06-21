/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "keyfinder.h"

namespace KeyFinder {

  KeyDetectionResult KeyFinder::keyOfAudio(
    const AudioData& originalAudio,
    const Parameters& params
  ) {
    Workspace workspace;

    progressiveChromagram(originalAudio, workspace, params);
    finalChromagram(workspace, params);

    return keyOfChromagram(workspace, params);
  }

  void KeyFinder::progressiveChromagram(
    AudioData audio,
    Workspace& workspace,
    const Parameters& params
  ) {
    preprocess(audio, workspace, params);
    workspace.preprocessedBuffer.append(audio);
    chromagramOfBufferedAudio(workspace, params);
  }

  void KeyFinder::finalChromagram(
    Workspace& workspace,
    const Parameters& params
  ) {
    // flush remainder buffer
    if (workspace.remainderBuffer.getSampleCount() > 0) {
      AudioData flush;
      preprocess(flush, workspace, params, true);
    }
    // zero padding
    unsigned int paddedHopCount = ceil(workspace.preprocessedBuffer.getSampleCount() / (float)params.getHopSize());
    unsigned int finalSampleLength = params.getFftFrameSize() + ((paddedHopCount - 1) * params.getHopSize());
    workspace.preprocessedBuffer.addToSampleCount(finalSampleLength - workspace.preprocessedBuffer.getSampleCount());
    chromagramOfBufferedAudio(workspace, params);
  }

  KeyDetectionResult KeyFinder::keyOfChromagram(
    Workspace& workspace,
    const Parameters& params
  ) const {

    KeyDetectionResult result;

    // working copy of chromagram
    Chromagram ch(*workspace.chromagram);
    ch.reduceToOneOctave();

    // get harmonic change signal and segment
    Segmentation segmenter;
    std::vector<unsigned int> segmentBoundaries = segmenter.getSegmentationBoundaries(ch, params);
    segmentBoundaries.push_back(ch.getHops()); // sentinel

    // get key estimates for each segment
    KeyClassifier classifier(
      params.getSimilarityMeasure(),
      params.getToneProfile(),
      params.getOffsetToC(),
      params.getCustomToneProfile()
    );

    std::vector<float> keyWeights(24); // TODO: not ideal using int cast of key_t enum. Hash?

    for (int s = 0; s < (signed) segmentBoundaries.size() - 1; s++) {
      KeyDetectionResultSegment segment;
      segment.firstHop = segmentBoundaries[s];
      segment.lastHop  = segmentBoundaries[s+1] - 1;
      // collapse segment's time dimension
      std::vector<float> segmentChroma(ch.getBands(), 0.0);
      for (unsigned int hop = segment.firstHop; hop <= segment.lastHop; hop++) {
        for (unsigned int band = 0; band < ch.getBands(); band++) {
          float value = ch.getMagnitude(hop, band);
          segmentChroma[band] += value;
          segment.energy += value;
        }
      }
      segment.chromaVector = segmentChroma;
      segment.key = classifier.classify(segmentChroma);
      if (segment.key != SILENCE)
        keyWeights[segment.key] += segment.energy;
      result.segments.push_back(segment);
    }

    // get global key
    result.globalKeyEstimate = SILENCE;
    float mostCommonKeyWeight = 0.0;
    for (int k = 0; k < (signed)keyWeights.size(); k++) {
      if (keyWeights[k] > mostCommonKeyWeight) {
        mostCommonKeyWeight = keyWeights[k];
        result.globalKeyEstimate = (key_t)k;
      }
    }

    return result;
  }

  void KeyFinder::preprocess(
    AudioData& workingAudio,
    Workspace& workspace,
    const Parameters& params,
    bool flushRemainderBuffer
  ) {
    workingAudio.reduceToMono();

    if (workspace.remainderBuffer.getChannels() > 0) {
      workingAudio.prepend(workspace.remainderBuffer);
      workspace.remainderBuffer.discardFramesFromFront(workspace.remainderBuffer.getFrameCount());
    }

    // TODO: there is presumably some good maths to determine filter frequencies.
    // For now, this approximates original experiment values for default params.
    float lpfCutoff = params.getLastFrequency() * 1.012;
    float dsCutoff = params.getLastFrequency() * 1.10;
    unsigned int downsampleFactor = (int) floor(workingAudio.getFrameRate() / 2 / dsCutoff);

    if (!flushRemainderBuffer && workingAudio.getSampleCount() % downsampleFactor != 0) {
      AudioData* remainder = workingAudio.sliceSamplesFromBack(workingAudio.getSampleCount() % downsampleFactor);
      workspace.remainderBuffer.append(*remainder);
      delete remainder;
    }

    // get filter
    const LowPassFilter* lpf = lpfFactory.getLowPassFilter(160, workingAudio.getFrameRate(), lpfCutoff, 2048);
    lpf->filter(workingAudio, workspace, downsampleFactor); // downsampleFactor shortcut
    // note we don't delete the LPF; it's stored in the factory for reuse

    workingAudio.downsample(downsampleFactor);
  }

  void KeyFinder::chromagramOfBufferedAudio(
    Workspace& workspace,
    const Parameters& params
  ) {
    if (workspace.fftAdapter == NULL)
      workspace.fftAdapter = new FftAdapter(params.getFftFrameSize());
    SpectrumAnalyser sa(workspace.preprocessedBuffer.getFrameRate(), params, &ctFactory, &twFactory);
    Chromagram* c = sa.chromagramOfWholeFrames(workspace.preprocessedBuffer, workspace.fftAdapter);
    // deal with tuning if necessary
    if (c->getBandsPerSemitone() > 1) {
      if (params.getTuningMethod() == TUNING_BAND_ADAPTIVE) {
        c->tuningBandAdaptive(params.getDetunedBandWeight());
      } else if (params.getTuningMethod() == TUNING_HARTE) {
        c->tuningHarte();
      }
    }
    workspace.preprocessedBuffer.discardFramesFromFront(params.getHopSize() * c->getHops());
    if (workspace.chromagram == NULL) {
      workspace.chromagram = c;
    } else {
      workspace.chromagram->append(*c);
      delete c;
    }
  }

}
