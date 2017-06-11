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

#include "fftadapter.h"

// Included here to allow substitution of a separate implementation .cpp
#include <cmath>
#include <cstring>
#include <fftw3.h>

namespace KeyFinder {

  std::mutex fftwPlanMutex;

  class FftAdapterPrivate {
  public:
    double* inputReal;
    fftw_complex* outputComplex;
    fftw_plan plan;
  };

  FftAdapter::FftAdapter(unsigned int inFrameSize) : priv(new FftAdapterPrivate) {
    frameSize = inFrameSize;
    priv->inputReal = (double*)fftw_malloc(sizeof(double) * frameSize);
    priv->outputComplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * frameSize);
    memset(priv->outputComplex, 0, sizeof(fftw_complex) * frameSize);
    fftwPlanMutex.lock();
    priv->plan = fftw_plan_dft_r2c_1d(frameSize, priv->inputReal, priv->outputComplex, FFTW_ESTIMATE);
    fftwPlanMutex.unlock();
  }

  FftAdapter::~FftAdapter() {
    fftw_destroy_plan(priv->plan);
    fftw_free(priv->inputReal);
    fftw_free(priv->outputComplex);
    delete priv;
  }

  unsigned int FftAdapter::getFrameSize() const {
    return frameSize;
  }

  void FftAdapter::setInput(unsigned int i, double real) {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(real)) {
      throw Exception("Cannot set sample to NaN");
    }
    priv->inputReal[i] = real;
  }

  double FftAdapter::getOutputReal(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return priv->outputComplex[i][0];
  }

  double FftAdapter::getOutputImaginary(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return priv->outputComplex[i][1];
  }

  double FftAdapter::getOutputMagnitude(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    return sqrt( pow(getOutputReal(i), 2) + pow(getOutputImaginary(i), 2) );
  }

  void FftAdapter::execute() {
    fftw_execute(priv->plan);
  }

  // ================================= INVERSE =================================

  class InverseFftAdapterPrivate {
  public:
    fftw_complex* inputComplex;
    double* outputReal;
    fftw_plan plan;
  };

  InverseFftAdapter::InverseFftAdapter(unsigned int inFrameSize) : priv(new InverseFftAdapterPrivate) {
    frameSize = inFrameSize;
    priv->inputComplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * frameSize);
    priv->outputReal = (double*)fftw_malloc(sizeof(double) * frameSize);
    fftwPlanMutex.lock();
    priv->plan = fftw_plan_dft_c2r_1d(frameSize, priv->inputComplex, priv->outputReal, FFTW_ESTIMATE);
    fftwPlanMutex.unlock();
  }

  InverseFftAdapter::~InverseFftAdapter() {
    fftw_destroy_plan(priv->plan);
    fftw_free(priv->inputComplex);
    fftw_free(priv->outputReal);
    delete priv;
  }

  unsigned int InverseFftAdapter::getFrameSize() const {
    return frameSize;
  }

  void InverseFftAdapter::setInput(unsigned int i, double real, double imag) {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(real) || !std::isfinite(imag)) {
      throw Exception("Cannot set sample to NaN");
    }
    priv->inputComplex[i][0] = real;
    priv->inputComplex[i][1] = imag;
  }

  double InverseFftAdapter::getOutput(unsigned int i) const {
    if (i >= frameSize) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << i << "/" << frameSize << ")";
      throw Exception(ss.str().c_str());
    }
    // divide by frameSize to normalise
    return priv->outputReal[i] / frameSize;
  }

  void InverseFftAdapter::execute() {
    fftw_execute(priv->plan);
  }

}
