#include "keyfinder.h"

namespace KeyFinder {

  KeyDetectionResult KeyFinder::keyOfAudio(
    const AudioData& originalAudio,
    const Parameters& params
  ) {
    AudioData buffer;
    Chromagram c = progressiveChromagramOfAudio(originalAudio, buffer, params);
    c.append(finalChromagramOfAudio(buffer, params));
    return keyOfChromagram(c, params);
  }

  Chromagram KeyFinder::progressiveChromagramOfAudio(
    const AudioData& originalAudio,
    AudioData& preprocessedBuffer,
    const Parameters& params
  ) {
    AudioData workingAudio(originalAudio);
    preprocess(workingAudio, params);
    preprocessedBuffer.append(workingAudio);
    return chromagramOfBufferedAudio(preprocessedBuffer, params);
  }

  Chromagram KeyFinder::finalChromagramOfAudio(
    AudioData& preprocessedBuffer,
    const Parameters& params
  ) {
    // zero padding
    unsigned int paddedHopCount = ceil(preprocessedBuffer.getSampleCount() / (float)params.getHopSize());
    unsigned int finalSampleLength = params.getFftFrameSize() + ((paddedHopCount - 1) * params.getHopSize());
    while (preprocessedBuffer.getSampleCount() < finalSampleLength)
      preprocessedBuffer.addToSampleCount(1);
    return chromagramOfBufferedAudio(preprocessedBuffer, params);
  }

  KeyDetectionResult KeyFinder::keyOfChromagram(
    const Chromagram& chromagram,
    const Parameters& params
  ) {

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
    const Parameters& params
  ) {
    workingAudio.reduceToMono();

    // TODO: there is presumably some good maths to determine filter frequencies.
    // For now, this approximates original experiment values for default params.
    float lpfCutoff = params.getLastFrequency() * 1.012;
    float dsCutoff = params.getLastFrequency() * 1.10;
    unsigned int downsampleFactor = (int)floor( workingAudio.getFrameRate() / 2 / dsCutoff );

    // get filter
    LowPassFilter* lpf = lpfFactory.getLowPassFilter(160, workingAudio.getFrameRate(), lpfCutoff, 2048);
    lpf->filter(workingAudio, downsampleFactor); // downsampleFactor shortcut
    // note we don't delete the LPF; it's stored in the factory for reuse

    Downsampler ds;
    ds.downsample(workingAudio, downsampleFactor);
  }

  Chromagram KeyFinder::chromagramOfBufferedAudio(
    AudioData& preprocessedBuffer,
    const Parameters& params
  ) {
    SpectrumAnalyser sa(preprocessedBuffer.getFrameRate(), params, ctFactory);
    Chromagram c;
    while (preprocessedBuffer.getSampleCount() >= params.getFftFrameSize()) {
      Chromagram working = sa.chromagramOfFirstFrame(preprocessedBuffer);
      // deal with tuning if necessary
      if (working.getBandsPerSemitone() > 1) {
        if (params.getTuningMethod() == TUNING_BAND_ADAPTIVE) {
          working.tuningBandAdaptive(params.getDetunedBandWeight());
        } else if (params.getTuningMethod() == TUNING_HARTE) {
          working.tuningHarte();
        }
      }
      c.append(working);
      preprocessedBuffer.discardFramesFromFront(params.getHopSize());
    }
    return c;
  }

}
