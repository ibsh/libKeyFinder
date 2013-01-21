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

#include "spectrumanalyser.h"

namespace KeyFinder{

  SpectrumAnalyser::SpectrumAnalyser(unsigned int f, const Parameters& params, ChromaTransformFactory* spFactory){
    bins = params.getOctaves() * params.getBpo();
    hopSize = params.getHopSize();
    frameRate = f;
    fftFrameSize = params.getFftFrameSize();
    ct = spFactory->getChromaTransform(f, params);
    fftInput  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    fftOutput = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fftFrameSize);
    fftPlan = fftw_plan_dft_1d(fftFrameSize, fftInput, fftOutput, FFTW_FORWARD, FFTW_ESTIMATE);
    wf = WindowFunction::getWindowFunction(params.getTemporalWindow());
  }

  SpectrumAnalyser::~SpectrumAnalyser(){
    // don't delete the chroma transform; it's stored in the factory
    delete wf;
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput);
    fftw_free(fftOutput);
  }

  Chromagram* SpectrumAnalyser::chromagram(AudioData* audio){
    Chromagram* ch = new Chromagram((audio->getSampleCount()/hopSize) + 1,bins);
    for (unsigned int i = 0; i < audio->getSampleCount(); i += hopSize){
      for (unsigned int j = 0; j < fftFrameSize; j++){
        if(i+j < audio->getSampleCount())
          fftInput[j][0] = audio->getSample(i+j) * wf->window(j,fftFrameSize); // real part, windowed
        else
          fftInput[j][0] = 0.0; // zero-pad if no PCM data remaining
        fftInput[j][1] = 0.0;   // zero out imaginary part
      }
      fftw_execute(fftPlan);
      std::vector<float> cv = ct->chromaVector(fftOutput);
      for (unsigned int j=0; j<bins; j++)
        ch->setMagnitude(i/hopSize,j,cv[j]);
    }
    return ch;
  }

}
