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

#ifndef FFTPOSTPROCESSOR_H
#define FFTPOSTPROCESSOR_H

#include <fftw3.h>
#include "parameters.h"

namespace KeyFinder{

  class FftPostProcessor{
  public:
    static FftPostProcessor* getFftPostProcessor(unsigned int, const Parameters&);
    FftPostProcessor(unsigned int, const Parameters&);
    virtual ~FftPostProcessor();
    virtual std::vector<float> chromaVector(fftw_complex*) const = 0;
  protected:
    unsigned int bins;
    unsigned int fftFrameSize;
    unsigned int frameRate;
  };

}

#endif
