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
    ct = spFactory->getChromaTransform(f, params);
    wf = WindowFunction::getWindowFunction(params.getTemporalWindow());
    fft = new FftAdapter(params.getFftFrameSize());
  }

  SpectrumAnalyser::~SpectrumAnalyser(){
    // don't delete the chroma transform; it's stored in the factory
    delete wf;
    delete fft;
  }

  Chromagram* SpectrumAnalyser::chromagram(AudioData* audio){
    Chromagram* ch = new Chromagram((audio->getSampleCount()/hopSize) + 1,bins);
    unsigned int fftFrameSize = fft->getFrameSize();
    for (unsigned int i = 0; i < audio->getSampleCount(); i += hopSize){
      for (unsigned int j = 0; j < fftFrameSize; j++){
        if(i+j < audio->getSampleCount()){
          fft->setInput(j, audio->getSample(i+j) * wf->window(j,fftFrameSize)); // real part, windowed
        }else{
          fft->setInput(j, 0.0); // zero-pad if no PCM data remaining
        }
      }
      fft->execute();
      std::vector<float> cv = ct->chromaVector(fft);
      for (unsigned int j=0; j<bins; j++){
        ch->setMagnitude(i/hopSize, j, cv[j]);
      }
    }
    return ch;
  }

}
