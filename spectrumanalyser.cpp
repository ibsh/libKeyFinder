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

  SpectrumAnalyser::SpectrumAnalyser(unsigned int f, const Parameters& params, ChromaTransformFactory* spFactory) {
    octaves = params.getOctaves();
    bandsPerSemitone = params.getBandsPerSemitone();
    hopSize = params.getHopSize();
    ct = spFactory->getChromaTransform(f, params);
    unsigned int fftFrameSize = params.getFftFrameSize();
    fft = new FftAdapter(fftFrameSize);
    WindowFunction win;
    temporalWindow = std::vector<float>(fftFrameSize);
    for (unsigned int i = 0; i < fftFrameSize; i++) {
      temporalWindow[i] = win.window(params.getTemporalWindow(), i, fftFrameSize);
    }
  }

  SpectrumAnalyser::~SpectrumAnalyser() {
    // don't delete the chroma transform; it's stored in the factory
    delete fft;
  }

  Chromagram SpectrumAnalyser::chromagram(AudioData* audio) const {
    if (audio->getChannels() != 1)
      throw Exception("Audio must be monophonic to be analysed");
    unsigned int sampleCount = audio->getSampleCount();
    unsigned int hops = (sampleCount / hopSize);
    if (sampleCount % hopSize > 0) hops++;
    Chromagram ch(hops, octaves, bandsPerSemitone);
    unsigned int fftFrameSize = fft->getFrameSize();
    for (unsigned int hop = 0; hop < hops; hop ++) {
      unsigned int sampleOffset = hop * hopSize;
      for (unsigned int sample = 0; sample < fftFrameSize; sample++) {
        if (sampleOffset + sample < sampleCount) {
          fft->setInput(sample, audio->getSample(sampleOffset + sample) * temporalWindow[sample]); // real part, windowed
        } else {
          fft->setInput(sample, 0.0); // zero-pad if no PCM data remaining
        }
      }
      fft->execute();
      std::vector<float> cv = ct->chromaVector(fft);
      for (unsigned int band = 0; band < ch.getBands(); band++) {
        ch.setMagnitude(hop, band, cv[band]);
      }
    }
    return ch;
  }

}
