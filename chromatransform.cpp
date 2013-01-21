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

#include "chromatransform.h"

namespace KeyFinder{

  ChromaTransform::ChromaTransform(unsigned int fr, const Parameters& params){
    frameRate = fr;
    bins = params.getOctaves() * params.getBpo();
    fftFrameSize = params.getFftFrameSize();
    // TODO check for sufficient low end resolution.
    if(frameRate < 1){
      throw Exception("Frame rate must be > 0");
    }
    if(params.getLastFreq() > frameRate / 2.0){
      throw Exception("Analysis frequencies specified over Nyquist");
    }
    binOffsets.resize(bins);
    directSpectralKernel.resize(bins);
    float myQFactor = params.getDirectSkStretch() * (pow(2,(1.0 / params.getBpo()))-1);
    for (unsigned int i = 0; i < bins; i++){
      float centreOfWindow = params.getBinFreq(i) * fftFrameSize / fr;
      float widthOfWindow = centreOfWindow * myQFactor;
      float beginningOfWindow = centreOfWindow - (widthOfWindow / 2);
      float endOfWindow = beginningOfWindow + widthOfWindow;
      float sumOfCoefficients = 0.0;
      binOffsets[i] = ceil(beginningOfWindow); // first useful fft bin
      for (
        unsigned int thisFftBin = ceil(beginningOfWindow);
        thisFftBin <= floor(endOfWindow);
        thisFftBin++
      ){
        float coefficient = kernelWindow(thisFftBin - beginningOfWindow, widthOfWindow);
        sumOfCoefficients += coefficient;
        directSpectralKernel[i].push_back(coefficient);
      }
      // normalisation by sum of coefficients and frequency of bin; models CQT very closely
      for (unsigned int j = 0; j < directSpectralKernel[i].size(); j++)
        directSpectralKernel[i][j] = directSpectralKernel[i][j] / sumOfCoefficients * params.getBinFreq(i);
    }
  }

  float ChromaTransform::kernelWindow(float n, float N)const{
    // discretely sampled continuous function, but different to other window functions
    return 1.0 - cos((2 * PI * n)/N); // based on Hann; no need to halve since we normalise later
  }

  std::vector<float> ChromaTransform::chromaVector(fftw_complex* fftResult) const{
    std::vector<float> cv(bins);
    for (unsigned int i = 0; i < bins; i++){
      float sum = 0.0;
      for (unsigned int j = 0; j < directSpectralKernel[i].size(); j++){
        unsigned int binNum = binOffsets[i]+j;
        float real = fftResult[binNum][0];
        float imag = fftResult[binNum][1];
        float magnitude = sqrt((real*real)+(imag*imag));
        sum += (magnitude * directSpectralKernel[i][j]);
      }
      cv[i] = sum;
    }
    return cv;
  }

}
