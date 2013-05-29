#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "chromatransformfactory.h"
#include "downsampler.h"
#include "keyclassifier.h"
#include "keyfinderresult.h"
#include "lowpassfilterfactory.h"
#include "parameters.h"
#include "segmentation.h"
#include "spectrumanalyser.h"
#include "workspace.h"

namespace KeyFinder {

  class KeyFinder {
  public:
    void progressiveChromagram(
      AudioData audio,
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    void finalChromagram(
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    KeyDetectionResult keyOfChromagram(
      Workspace& workspace,
      const Parameters& params = Parameters()
    ) const;
    KeyDetectionResult keyOfAudio(
      const AudioData& audio,
      const Parameters& params = Parameters()
    );
  private:
    void preprocess(
      AudioData& workingAudio,
      Workspace& workspace,
      const Parameters& params
    );
    void chromagramOfBufferedAudio(
      Workspace& workspace,
      const Parameters& params = Parameters()
    );
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
