#include "keyfinder.h"

namespace KeyFinder{

  KeyDetectionResult KeyFinder::findKey(AudioData& audio, const Parameters& params){

    KeyDetectionResult result;

    // make audio stream monaural
    audio.reduceToMono();

    // get spectrum analyser
    SpectrumAnalyser* sa = saFactory.getSpectrumAnalyser(audio.getFrameRate(), params);

    // run spectrum analysis
    Chromagram* ch = sa->chromagram(audio);

    // reduce chromagram
    ch->reduceTuningBins(params);
    result.fullChromagram = Chromagram(*ch);
    ch->reduceToOneOctave(params);
    result.oneOctaveChromagram = Chromagram(*ch);

    // get harmonic change signal
    Segmentation* segmenter = Segmentation::getSegmentation(params);
    result.harmonicChangeSignal = segmenter->getRateOfChange(*ch, params);

    // get track segmentation
    std::vector<unsigned int> segmentBoundaries = segmenter->getSegments(result.harmonicChangeSignal, params);
    segmentBoundaries.push_back(ch->getHops()); // sentinel
    delete segmenter;

    // get key estimates for each segment
    KeyClassifier hc(params);
    std::vector<float> keyWeights(24); // TODO: not ideal using int cast of key_t enum

    for (int s = 0; s < (signed)segmentBoundaries.size()-1; s++){
      KeyDetectionSegment segment;
      segment.firstWindow = segmentBoundaries[s];
      segment.lastWindow = segmentBoundaries[s+1] - 1;
      // collapse segment's time dimension, for a single chroma vector and a single energy value
      std::vector<float> segmentChroma(ch->getBins());
      // for each relevant hop of the chromagram
      for (unsigned int hop = segment.firstWindow; hop <= segment.lastWindow; hop++) {
        // for each bin
        for (unsigned int bin = 0; bin < ch->getBins(); bin++) {
          float value = ch->getMagnitude(hop, bin);
          segmentChroma[bin] += value;
          segment.energy += value;
        }
      }
      segment.key = hc.classify(segmentChroma);
      if(segment.key != SILENCE){
        keyWeights[segment.key] += segment.energy;
      }
      result.segments.push_back(segment);
    }

    delete ch;

    // get global key
    result.globalKeyEstimate = SILENCE;
    float mostCommonKeyWeight = 0.0;
    for (int k = 0; k < (signed)keyWeights.size(); k++){
      if(keyWeights[k] > mostCommonKeyWeight){
        mostCommonKeyWeight = keyWeights[k];
        result.globalKeyEstimate = (key_t)k;
      }
    }

    return result;

  }

} // namespace
