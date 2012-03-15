#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "parameters.h"
#include "spectrumanalyserfactory.h"
#include "keyfinderresult.h"
#include "seg.h"
#include "keyclassifier.h"

namespace KeyFinder{

  class KeyFinder {
  public:
    KeyDetectionResult findKey(AudioData&, const Parameters&);
  private:
    SpectrumAnalyserFactory saFactory;
  };

} // namespace

#endif // KEYFINDER_H
