#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "parameters.h"
#include "lowpassfilterfactory.h"
#include "chromatransformfactory.h"
#include "downsampler.h"
#include "spectrumanalyser.h"
#include "keyfinderresult.h"
#include "seg.h"
#include "keyclassifier.h"

namespace KeyFinder{

  class KeyFinder {
  public:
    KeyDetectionResult findKey(const AudioData&, const Parameters&);
  private:
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
  };

}

#endif
