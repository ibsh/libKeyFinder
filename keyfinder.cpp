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
    workspace.buffer.append(audio);
    chromagramOfBufferedAudio(workspace, params);
  }

  void KeyFinder::finalChromagram(
    Workspace& workspace,
    const Parameters& params
  ) {
    // zero padding
    unsigned int paddedHopCount = ceil(workspace.buffer.getSampleCount() / (float)params.getHopSize());
    unsigned int finalSampleLength = params.getFftFrameSize() + ((paddedHopCount - 1) * params.getHopSize());
    workspace.buffer.addToSampleCount(finalSampleLength - workspace.buffer.getSampleCount());
    chromagramOfBufferedAudio(workspace, params);
  }

  KeyDetectionResult KeyFinder::keyOfChromagram(
    Workspace& workspace,
    const Parameters& params
  ) const {

    KeyDetectionResult result;

    // working copy of chromagram
    Chromagram* ch = new Chromagram(*workspace.chroma);
    ch->reduceToOneOctave();

    // get harmonic change signal and segment
    Segmentation segmenter;
    std::vector<unsigned int> segmentBoundaries = segmenter.getSegmentationBoundaries(ch, params);
    segmentBoundaries.push_back(ch->getHops()); // sentinel

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
      std::vector<float> segmentChroma(ch->getBands(), 0.0);
      for (unsigned int hop = segment.firstHop; hop <= segment.lastHop; hop++) {
        for (unsigned int band = 0; band < ch->getBands(); band++) {
          float value = ch->getMagnitude(hop, band);
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

    delete ch;

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

  void KeyFinder::chromagramOfBufferedAudio(
    Workspace& workspace,
    const Parameters& params
  ) {
    if (workspace.getFftAdapter() == NULL)
      workspace.setFftAdapter(new FftAdapter(params.getFftFrameSize()));
    SpectrumAnalyser sa(workspace.buffer.getFrameRate(), params, ctFactory);
    Chromagram* c = sa.chromagramOfWholeFrames(workspace.buffer, workspace.getFftAdapter());
    // deal with tuning if necessary
    if (c->getBandsPerSemitone() > 1) {
      if (params.getTuningMethod() == TUNING_BAND_ADAPTIVE) {
        c->tuningBandAdaptive(params.getDetunedBandWeight());
      } else if (params.getTuningMethod() == TUNING_HARTE) {
        c->tuningHarte();
      }
    }
    workspace.buffer.discardFramesFromFront(params.getHopSize() * c->getHops());
    if (workspace.chroma == NULL) {
      workspace.chroma = c;
    } else {
      workspace.chroma->append(*c);
      delete c;
    }
  }

}
