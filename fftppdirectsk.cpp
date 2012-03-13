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

#include "fftppdirectsk.h"

namespace KeyFinder{

  DirectSkPostProc::DirectSkPostProc(unsigned int frameRate, const Parameters& prefs) : FftPostProcessor(frameRate, prefs) {
    pi = (4 * atan(1.0));
    binOffsets = std::vector<unsigned int>(bins);
    mySpecKernel = std::vector<std::vector<float> >(bins,std::vector<float>(0));
    float myQFactor = prefs.getDirectSkStretch() * (pow(2,(1.0 / prefs.getBpo()))-1);
    for (unsigned int i=0; i<bins; i++){
      float centreOfWindow = prefs.getBinFreq(i) * fftFrameSize / frameRate;
      float widthOfWindow = centreOfWindow * myQFactor;
      float beginningOfWindow = centreOfWindow - (widthOfWindow / 2);
      float endOfWindow = beginningOfWindow + widthOfWindow;
      float sumOfCoefficients = 0.0;
      for (unsigned int thisFftBin = 0; thisFftBin < fftFrameSize; thisFftBin++){
        if((float)thisFftBin < beginningOfWindow)
          continue; // haven't got to useful fft bins yet
        if((float)thisFftBin > endOfWindow)
          break; // finished with useful fft bins
        if(binOffsets[i] == 0)
          binOffsets[i] = thisFftBin; // first useful fft bin
        float coefficient = kernelWindow(thisFftBin-beginningOfWindow,widthOfWindow);
        sumOfCoefficients += coefficient;
        mySpecKernel[i].push_back(coefficient);
      }
      // normalisation by sum of coefficients and frequency of bin; models CQT very closely
      for (unsigned int j = 0; j < mySpecKernel[i].size(); j++)
        mySpecKernel[i][j] = mySpecKernel[i][j] / sumOfCoefficients * prefs.getBinFreq(i);
    }
  }

  std::vector<float> DirectSkPostProc::chromaVector(fftw_complex* fftResult)const{
    std::vector<float> cv(bins);
    for (unsigned int i=0; i<bins; i++){
      float sum = 0.0;
      for (unsigned int j = 0; j < mySpecKernel[i].size(); j++){
        unsigned int binNum = binOffsets[i]+j;
        float real = fftResult[binNum][0];
        float imag = fftResult[binNum][1];
        float magnitude = sqrt((real*real)+(imag*imag));
        sum += (magnitude * mySpecKernel[i][j]);
      }
      cv[i] = sum;
    }
    return cv;
  }

  float DirectSkPostProc::kernelWindow(float n, float N)const{
    // discretely sampled continuous function, but different to other window functions
    return 1.0 - cos((2 * pi * n)/N); // based on Hann; no need to halve since we normalise later
  }

} // namespace
