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

#ifndef DIRECTSKPOSTPROC_H
#define DIRECTSKPOSTPROC_H

#include <math.h>
#include <vector>
#include "fftpp.h"
#include "parameters.h"
#include "windowfunctions.h"

namespace KeyFinder{

  class DirectSkPostProc : public FftPostProcessor{
  public:
    DirectSkPostProc(unsigned int, const Parameters&);
    virtual std::vector<float> chromaVector(fftw_complex*)const;
  private:
    std::vector<std::vector<float> > mySpecKernel; // ragged 2D array; narrow for bass, wide for treble.
    std::vector<unsigned int> binOffsets; // which fft bin to multiply by first coefficient.
    float kernelWindow(float,float)const;
  };

}

#endif
