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

#include "lowpassfilter.h"

namespace KeyFinder{

  LowPassFilter::LowPassFilter(unsigned int coefficientCount, unsigned int frameRate, float cornerFrequency, unsigned int fftFrameSize){
    float cutoffPoint = cornerFrequency / frameRate;
    fftw_complex* fftInput  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    fftw_complex* fftResult = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    fftw_plan fftPlan = fftw_plan_dft_1d(fftFrameSize, fftInput, fftResult, FFTW_FORWARD, FFTW_ESTIMATE);

    // Build frequency domain response
    float tau = 0.5 / cutoffPoint;
    for (unsigned int i = 0; i <= fftFrameSize/2; i++){
      if ((float) i / (float) fftFrameSize <= cutoffPoint){
        fftInput[i][0] = 1.0;
      } else {
        fftInput[i][0] = 0.0;
      }
      fftInput[i][0] *= tau;
      fftInput[i][1] = 0.0;
    }
    for (unsigned int i = 1; i < fftFrameSize/2; i++){
      fftInput[fftFrameSize-i][0] = fftInput[i][0];
      fftInput[fftFrameSize-i][1] = 0.0;
    }

    // inverse FFT to determine time-domain response
    fftw_execute(fftPlan);

    coefficients.resize(coefficientCount);
    unsigned int centre = (coefficientCount-1)/2;
    for (unsigned int i = 0; i < coefficientCount; i++){
      // Grabbing the very end and the very beginning of the real FFT output?
      unsigned int index = (fftFrameSize - centre + i) % fftFrameSize;
      coefficients[i] = fftResult[index][0] / (float) fftFrameSize;
    }

    // cleanup FFTW memory
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput);
    fftw_free(fftResult);

    WindowFunction* wf = WindowFunction::getWindowFunction(WINDOW_HAMMING);
    for (unsigned int i = 0; i < coefficientCount; i++){
      // apply window
      coefficients[i] *= wf->window(i, coefficientCount);
      // finalise gain
      gain += coefficients[i];
    }
    delete wf;

  }

}
