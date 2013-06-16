#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "parameters.h"
#include "lowpassfilterfactory.h"
#include "chromatransformfactory.h"
#include "spectrumanalyser.h"
#include "keyfinderresult.h"
#include "segmentation.h"
#include "keyclassifier.h"

namespace KeyFinder {

  class KeyFinder {
  public:
    Chromagram chromagramOfAudio(
      const AudioData& audio,
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
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
