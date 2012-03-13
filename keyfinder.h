#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "_GLOBAL.h"

#include "audiostream.h"
#include "parameters.h"
#include "spectrumanalyserfactory.h"
#include "keyfinderresult.h"
#include "seg.h"
#include "keyclassifier.h"

namespace KeyFinder{

  class LIBKEYFINDERSHARED_EXPORT KeyFinder {
  public:
    KeyDetectionResult findKey(AudioStream&, const Parameters&);
  private:
    SpectrumAnalyserFactory saFactory;
  };

} // namespace

#endif // KEYFINDER_H
