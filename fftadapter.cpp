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

#include "fftadapter.h"

namespace KeyFinder{

  FftAdapter::FftAdapter(unsigned int fs) {
    frameSize = fs;
    input  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*frameSize);
    output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*frameSize);
    plan = fftw_plan_dft_1d(frameSize, input, output, FFTW_FORWARD, FFTW_ESTIMATE);
  }

  FftAdapter::~FftAdapter() {
    fftw_destroy_plan(plan);
    fftw_free(input);
    fftw_free(output);
  }

  unsigned int FftAdapter::getFrameSize() const {
    return frameSize;
  }

  void FftAdapter::setInput(unsigned int i, float real) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    if (!boost::math::isfinite(real)) {
      throw Exception("Cannot set sample to NaN");
    }
    input[i][0] = real;
    input[i][1] = 0.0;
  }

  float FftAdapter::getOutputReal(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return output[i][0];
  }

  float FftAdapter::getOutputImaginary(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return output[i][1];
  }

  float FftAdapter::getOutputMagnitude(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return sqrt( pow(getOutputReal(i), 2) + pow(getOutputImaginary(i), 2) );
  }

  void FftAdapter::execute() {
    fftw_execute(plan);
  }

}
