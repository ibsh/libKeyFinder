/*************************************************************************

  Copyright 2012 Ibrahim Sha'ath

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

#ifndef FFTWANALYSER_H
#define FFTWANALYSER_H

#include "spectrumanalyser.h"
#include "fftpp.h"
#include "fftppdirectsk.h"
#include "windowfunctions.h"
#include <fftw3.h>

namespace KeyFinder{

  class FftwAnalyser : public SpectrumAnalyser{
  public:
    FftwAnalyser(unsigned int, const Parameters&);
    ~FftwAnalyser();
    virtual Chromagram* chromagram(const AudioStream&);
  private:
    unsigned int fftFrameSize;
    FftPostProcessor* pp;
    fftw_complex* fftInput;
    fftw_complex* fftResult;
    fftw_plan fftPlan;
    std::vector<float> window;
  };

} // namespace

#endif
