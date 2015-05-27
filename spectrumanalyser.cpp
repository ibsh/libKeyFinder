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

#include "spectrumanalyser.h"

namespace KeyFinder {

  SpectrumAnalyser::SpectrumAnalyser(unsigned int frameRate, ChromaTransformFactory* spFactory, TemporalWindowFactory* twFactory) {
    chromaTransform = spFactory->getChromaTransform(frameRate);
    tw = twFactory->getTemporalWindow(FFTFRAMESIZE);
  }

  Chromagram* SpectrumAnalyser::chromagramOfWholeFrames(AudioData& audio, FftAdapter* const fftAdapter) const {

    if (audio.getChannels() != 1) {
      throw Exception("Audio must be monophonic to be analysed");
    }

    unsigned int frmSize = fftAdapter->getFrameSize();
    if (audio.getSampleCount() < frmSize) {
      return new Chromagram(0);
    }

    unsigned int hops = 1 + ((audio.getSampleCount() - frmSize) / HOPSIZE);
    Chromagram* ch = new Chromagram(hops);

    for (unsigned int hop = 0; hop < hops; hop++) {

      audio.resetIterators();
      audio.advanceReadIterator(hop * HOPSIZE);

      std::vector<double>::const_iterator twIt = tw->begin();
      for (unsigned int sample = 0; sample < frmSize; sample++) {
        fftAdapter->setInput(sample, audio.getSampleAtReadIterator() * *twIt);
        audio.advanceReadIterator();
        std::advance(twIt, 1);
      }

      fftAdapter->execute();

      std::vector<double> cv = chromaTransform->chromaVector(fftAdapter);
      std::vector<double>::const_iterator cvIt = cv.begin();
      for (unsigned int band = 0; band < BANDS; band++) {
        ch->setMagnitude(hop, band, *cvIt);
        std::advance(cvIt, 1);
      }
    }
    return ch;
  }

}
