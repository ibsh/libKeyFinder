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
    KeyDetectionResult keyOfAudio(
      const AudioData& audio,
      const Parameters& params = Parameters()
    );
    Chromagram chromagramOfAudio(
      AudioData audio,
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    KeyDetectionResult keyOfChromagram(
      const Chromagram& chromagram,
      const Parameters& params = Parameters()
    ) const;
  private:
    void preprocess(
      AudioData& workingAudio,
      Workspace& workspace,
      const Parameters& params
    );
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
