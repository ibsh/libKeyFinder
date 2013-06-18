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

  // this method to be used for whole audio streams
  KeyDetectionResult KeyFinder::keyOfAudio(
    const AudioData& originalAudio,
    const Parameters& params
  ) {
    Workspace workspace;
    Chromagram ch = chromagramOfAudio(originalAudio, workspace, params);
    return keyOfChromagram(ch, params);
  }

  Chromagram KeyFinder::chromagramOfAudio(
    AudioData workingAudio,
    Workspace& workspace,
    const Parameters& params
  ) {
    preprocess(workingAudio, workspace, params);

    // run spectral analysis
    SpectrumAnalyser sa(workingAudio.getFrameRate(), params, &ctFactory);

    workspace.fftAdapter = new FftAdapter(params.getFftFrameSize());
    Chromagram ch = sa.chromagram(workingAudio, workspace.fftAdapter);

    // deal with tuning if necessary
    if (ch.getBandsPerSemitone() > 1) {
      if (params.getTuningMethod() == TUNING_BAND_ADAPTIVE) {
        ch.tuningBandAdaptive(params.getDetunedBandWeight());
      } else if (params.getTuningMethod() == TUNING_HARTE) {
        ch.tuningHarte();
      }
    }
    return ch;
  }

  KeyDetectionResult KeyFinder::keyOfChromagram(
    const Chromagram& chromagram,
    const Parameters& params
  ) const {

    KeyDetectionResult result;

    // working copy of chromagram
    Chromagram ch = Chromagram(chromagram);
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
      for (unsigned int hop = segment.firstHop; hop <= segment.lastHop; hop++) {
        for (unsigned int band = 0; band < ch.getBands(); band++) {
          float value = ch.getMagnitude(hop, band);
          segment.chromaVector[band] += value;
          segment.energy += value;
        }
      }
      segment.key = classifier.classify(segment.chromaVector);
      if (segment.key != SILENCE)
        keyWeights[segment.key] += segment.energy;
      result.segments.push_back(segment);
    }

    // get global key
    result.globalKeyEstimate = SILENCE;
    float mostCommonKeyWeight = 0.0;
    for (unsigned int k = 0; k < keyWeights.size(); k++) {
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
    const Parameters& params
  ) {
    workingAudio.reduceToMono();

    // TODO: there is presumably some good maths to determine filter frequencies.
    // For now, this approximates original experiment values for default params.
    float lpfCutoff = params.getLastFrequency() * 1.012;
    float dsCutoff = params.getLastFrequency() * 1.10;
    unsigned int downsampleFactor = (int) floor(workingAudio.getFrameRate() / 2 / dsCutoff);

    // get filter
    const LowPassFilter* lpf = lpfFactory.getLowPassFilter(160, workingAudio.getFrameRate(), lpfCutoff, 2048);
    lpf->filter(workingAudio, workspace, downsampleFactor); // downsampleFactor shortcut
    // note we don't delete the LPF; it's stored in the factory for reuse

    workingAudio.downsample(downsampleFactor);
  }

}
