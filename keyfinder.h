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
#include "workspace.h"

namespace KeyFinder {

  class KeyFinder {
  public:
    Chromagram progressiveChromagramOfAudio(
      const AudioData& audio,
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    Chromagram finalChromagramOfAudio(
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    KeyDetectionResult keyOfChromagram(
      const Chromagram& chromagram,
      const Parameters& params = Parameters()
    ) const;
    KeyDetectionResult keyOfAudio(
      const AudioData& audio,
      const Parameters& params = Parameters()
    );
  private:
    void preprocess(AudioData& workingAudio, const Parameters& params);
    Chromagram chromagramOfBufferedAudio(
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
