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

namespace KeyFinder {

  FftAdapter::FftAdapter(unsigned int fs) {
    frameSize = fs;
    inputReal = (double*)fftw_malloc(sizeof(double) * frameSize);
    outputComplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * frameSize);
    plan = fftw_plan_dft_r2c_1d(frameSize, inputReal, outputComplex, FFTW_ESTIMATE);
  }

  FftAdapter::~FftAdapter() {
    fftw_destroy_plan(plan);
    fftw_free(inputReal);
    fftw_free(outputComplex);
  }

  unsigned int FftAdapter::getFrameSize() const {
    return frameSize;
  }

  void FftAdapter::setInput(unsigned int i, float real) {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    if (!boost::math::isfinite(real)) {
      throw Exception("Cannot set sample to NaN");
    }
    inputReal[i] = real;
  }

  float FftAdapter::getOutputReal(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return outputComplex[i][0];
  }

  float FftAdapter::getOutputImaginary(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return outputComplex[i][1];
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

  // ================================= INVERSE =================================

  InverseFftAdapter::InverseFftAdapter(unsigned int fs) {
    frameSize = fs;
    inputComplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * frameSize);
    outputReal = (double*)fftw_malloc(sizeof(double) * frameSize);
    plan = fftw_plan_dft_c2r_1d(frameSize, inputComplex, outputReal, FFTW_ESTIMATE);
  }

  InverseFftAdapter::~InverseFftAdapter() {
    fftw_destroy_plan(plan);
    fftw_free(inputComplex);
    fftw_free(outputReal);
  }

  unsigned int InverseFftAdapter::getFrameSize() const {
    return frameSize;
  }

  void InverseFftAdapter::setInput(unsigned int i, float real, float imag) {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    if (!boost::math::isfinite(real) || !boost::math::isfinite(imag)) {
      throw Exception("Cannot set sample to NaN");
    }
    inputComplex[i][0] = real;
    inputComplex[i][1] = imag;
  }

  float InverseFftAdapter::getOutput(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    // divide by frameSize to normalise
    return outputReal[i] / frameSize;
  }

  void InverseFftAdapter::execute() {
    fftw_execute(plan);
  }

}
