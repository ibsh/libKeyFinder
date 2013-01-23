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

/*************************************************************************

  The low pass filter implementation is based on the work of Tony Fisher,
  as shown at http://www-users.cs.york.ac.uk/~fisher/mkfilter/

*************************************************************************/

#include "lowpassfilter.h"

namespace KeyFinder{

  LowPassFilter::LowPassFilter(unsigned int coefficientCount, unsigned int frameRate, float cornerFrequency, unsigned int fftFrameSize){
    float cutoffPoint = cornerFrequency / frameRate;
    FftAdapter* fft = new FftAdapter(fftFrameSize);

    // Build frequency domain response
    float tau = 0.5 / cutoffPoint;
    for (unsigned int i = 0; i <= fftFrameSize/2; i++){
      if ((float) i / (float) fftFrameSize <= cutoffPoint){
        fft->setInput(i, tau);
      } else {
        fft->setInput(i, 0.0);
      }
    }
    for (unsigned int i = 1; i < fftFrameSize/2; i++){
      fft->setInput(fftFrameSize-i, fft->getReal(i));
    }

    // inverse FFT to determine time-domain response
    fft->execute();

    coefficients.resize(coefficientCount);
    unsigned int centre = (coefficientCount-1)/2;
    for (unsigned int i = 0; i < coefficientCount; i++){
      // Grabbing the very end and the very beginning of the real FFT output?
      unsigned int index = (fftFrameSize - centre + i) % fftFrameSize;
      coefficients[i] = fft->getReal(index) / (float) fftFrameSize;
    }

    delete fft;

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
