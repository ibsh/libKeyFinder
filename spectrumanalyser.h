/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

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

#include <boost/thread/mutex.hpp>
#include "chromagram.h"
#include "audiodata.h"
#include "fftadapter.h"
#include "chromatransformfactory.h"
#include "parameters.h"
#include "windowfunctions.h"

namespace KeyFinder {

  class SpectrumAnalyser {
  public:
    SpectrumAnalyser(
      unsigned int frameRate,
      const Parameters& params,
      ChromaTransformFactory& ctFactory
    );
    Chromagram chromagramOfFirstFrame(const AudioData& audio) const;
    ~SpectrumAnalyser();
  protected:
    unsigned int octaves;
    unsigned int bandsPerSemitone;
    unsigned int hopSize;
    ChromaTransform* ct;
    FftAdapter* fft;
    std::vector<float> temporalWindow;
  };

}

#endif
