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

#include "spectrumanalyserfftw.h"

namespace KeyFinder{

  FftwAnalyser::FftwAnalyser(unsigned int f, const Parameters& params): SpectrumAnalyser(f, params) {
    fftFrameSize = params.getFftFrameSize();
    pp = FftPostProcessor::getFftPostProcessor(f, params);
    fftInput = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    fftResult = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    // plan 1-dimensional DFT
    fftPlan = fftw_plan_dft_1d(fftFrameSize, fftInput, fftResult, FFTW_FORWARD, FFTW_ESTIMATE);
    // prep temporal window function
    WindowFunction* wf = WindowFunction::getWindowFunction(params.getTemporalWindow());
    window = std::vector<float>(fftFrameSize);
    for (unsigned int i=0; i<fftFrameSize; i++){
      window[i] = wf->window(i,fftFrameSize);
    }
    delete wf;
  }

  FftwAnalyser::~FftwAnalyser(){
    delete pp;
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput);
    fftw_free(fftResult);
  }

  Chromagram* FftwAnalyser::chromagram(AudioData* audio){
    boost::mutex::scoped_lock lock(analyserMutex);
    Chromagram* ch = new Chromagram((audio->getSampleCount()/hopSize) + 1,bins);
    for (unsigned int i = 0; i < audio->getSampleCount(); i += hopSize){
      for (unsigned int j = 0; j < fftFrameSize; j++){
        if(i+j < audio->getSampleCount())
          fftInput[j][0] = audio->getSample(i+j) * window[j]; // real part, windowed
        else
          fftInput[j][0] = 0.0; // zero-pad if no PCM data remaining
        fftInput[j][1] = 0.0;   // zero out imaginary part
      }
      fftw_execute(fftPlan);
      std::vector<float> cv = pp->chromaVector(fftResult);
      for (unsigned int j=0; j<bins; j++)
        ch->setMagnitude(i/hopSize,j,cv[j]);
    }
    return ch;
  }

}
