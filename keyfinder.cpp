#include "keyfinder.h"

namespace KeyFinder{

  Chromagram KeyFinder::chromagramOfAudio(const AudioData& originalAudio, const Parameters& params){

    AudioData* workingAudio = new AudioData(originalAudio);

    workingAudio->reduceToMono();

    // TODO: there is presumably some good maths to determine filter frequencies
    float lpfCutoff = params.getLastFrequency() * 1.05;
    float dsCutoff = params.getLastFrequency() * 1.10;
    unsigned int downsampleFactor = (int)floor( workingAudio->getFrameRate() / 2 / dsCutoff );

    // get filter
    LowPassFilter* lpf = lpfFactory.getLowPassFilter(160, workingAudio->getFrameRate(), lpfCutoff, 2048);
    // feeding in the downsampleFactor for a shortcut
    lpf->filter(workingAudio, downsampleFactor);
    // note we don't delete the LPF; it's stored in the factory for reuse

    Downsampler ds;
    ds.downsample(workingAudio, downsampleFactor);

    // run spectral analysis
    SpectrumAnalyser sa(workingAudio->getFrameRate(), params, &ctFactory);
    Chromagram ch = sa.chromagram(workingAudio);

    delete workingAudio;

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

  KeyDetectionResult KeyFinder::keyOfChromagram(const Chromagram& chromagram, const Parameters& params){

    KeyDetectionResult result;

    // working copy of chromagram
    Chromagram ch = Chromagram(chromagram);
    ch.reduceToOneOctave();

    // get harmonic change signal and segment
    Segmentation* segmenter = Segmentation::getSegmentation(params);
    result.harmonicChangeSignal = segmenter->getRateOfChange(ch, params);
    std::vector<unsigned int> segmentBoundaries = segmenter->getSegments(result.harmonicChangeSignal, params);
    segmentBoundaries.push_back(ch.getHops()); // sentinel
    delete segmenter;

    // get key estimates for each segment
    KeyClassifier classifier(
      params.getSimilarityMeasure(),
      params.getToneProfile(),
      params.getOffsetToC(),
      params.getCustomToneProfile()
    );

    std::vector<float> keyWeights(24); // TODO: not ideal using int cast of key_t enum. Hash?

    for (int s = 0; s < (signed) segmentBoundaries.size() - 1; s++) {
      KeyDetectionSegment segment;
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

  // this method to be used for whole audio streams
  KeyDetectionResult KeyFinder::keyOfAudio(const AudioData& originalAudio, const Parameters& params){
    Chromagram ch = chromagramOfAudio(originalAudio, params);
    return keyOfChromagram(ch, params);
  }

}
