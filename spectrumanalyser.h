/*************************************************************************

  Copyright 2011-2015 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include "chromagram.h"
#include "audiodata.h"
#include "fftadapter.h"
#include "chromatransformfactory.h"
#include "constants.h"
#include "temporalwindowfactory.h"
#include "windowfunctions.h"

namespace KeyFinder {

  class SpectrumAnalyser {
  public:
    SpectrumAnalyser(unsigned int frameRate, ChromaTransformFactory* ctFactory, TemporalWindowFactory* twFactory);
    Chromagram* chromagramOfWholeFrames(AudioData& audio, FftAdapter* const fft) const;
  protected:
    const ChromaTransform* chromaTransform;
    const std::vector<double>* tw;
  };

}

#endif
