#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "parameters.h"
#include "lowpassfilterfactory.h"
#include "chromatransformfactory.h"
#include "downsampler.h"
#include "spectrumanalyser.h"
#include "keyfinderresult.h"
#include "segmentation.h"
#include "keyclassifier.h"

namespace KeyFinder {

  class KeyFinder {
  public:
    Chromagram progressiveChromagramOfAudio(
      const AudioData& audio,
      AudioData& preprocessedBuffer,
      const Parameters& params = Parameters()
    );
    Chromagram finalChromagramOfAudio(
      AudioData& preprocessedBuffer,
      const Parameters& params = Parameters()
    );
    KeyDetectionResult keyOfChromagram(
      const Chromagram& chromagram,
      const Parameters& params = Parameters()
    );
    KeyDetectionResult keyOfAudio(
      const AudioData& audio,
      const Parameters& params = Parameters()
    );
  private:
    void preprocess(AudioData& workingAudio, const Parameters& params);
    Chromagram chromagramOfBufferedAudio(
      AudioData& preprocessedBuffer,
      const Parameters& params = Parameters()
    );
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
