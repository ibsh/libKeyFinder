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

#ifndef FFTADAPTER_H
#define FFTADAPTER_H

#include <cmath>
#include <fftw3.h>
#include <boost/math/special_functions/fpclassify.hpp>
#include "exception.h"

namespace KeyFinder{

  class FftAdapter{
  public:
    FftAdapter(unsigned int);
    ~FftAdapter();
    unsigned int getFrameSize() const;
    void setInput(unsigned int, float) const;
    void execute();
    float getOutputReal(unsigned int) const;
    float getOutputImaginary(unsigned int) const;
    float getOutputMagnitude(unsigned int) const;
  protected:
    unsigned int frameSize;
    fftw_complex* input;
    fftw_complex* output;
    fftw_plan     plan;
  };

}

#endif
