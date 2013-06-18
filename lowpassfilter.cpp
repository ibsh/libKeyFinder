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

namespace KeyFinder {

  LowPassFilter::LowPassFilter(unsigned int ord, unsigned int frameRate, float cornerFrequency, unsigned int fftFrameSize) {
    if (ord % 2 != 0) throw Exception("LPF order must be an even number");
    if (ord > fftFrameSize / 4) throw Exception("LPF order must be <= FFT frame size / 4");
    order = ord;
    delay = order / 2;
    impulseLength = order + 1;
    float cutoffPoint = cornerFrequency / frameRate;
    InverseFftAdapter* ifft = new InverseFftAdapter(fftFrameSize);

    // Build frequency domain response
    float tau = 0.5 / cutoffPoint;
    for (unsigned int i = 0; i < fftFrameSize/2; i++) {
      float input = 0.0;
      if (i / (float) fftFrameSize <= cutoffPoint) input = tau;
      ifft->setInput(i, input, 0.0);
      ifft->setInput(fftFrameSize - i - 1, input, 0.0);
    }

    // inverse FFT to determine time-domain response
    ifft->execute();

    // TODO determine whether to handle bad_alloc
    coefficients.resize(impulseLength, 0.0);
    unsigned int centre = order / 2;
    gain = 0.0;
    WindowFunction win;

    for (unsigned int i = 0; i < impulseLength; i++) {
      // Grabbing the very end and the very beginning of the real FFT output?
      unsigned int index = (fftFrameSize - centre + i) % fftFrameSize;
      float coeff = ifft->getOutput(index);
      coeff *= win.window(WINDOW_HAMMING, i, impulseLength);
      coefficients[i] = coeff;
      gain += coeff;
    }

    delete ifft;
  }

  void LowPassFilter::filter(AudioData& audio, Workspace& workspace, unsigned int shortcutFactor) const {

    if (audio.getChannels() > 1) throw Exception("Monophonic audio only");

    std::vector<float>* buffer = workspace.lpfBuffer;

    if (buffer == NULL) {
      workspace.lpfBuffer = new std::vector<float>(impulseLength, 0.0);
      buffer = workspace.lpfBuffer;
    } else {
      // clear delay buffer
      std::vector<float>::iterator bufferIterator = buffer->begin();
      while (bufferIterator < buffer->end()) {
        *bufferIterator = 0.0;
        std::advance(bufferIterator, 1);
      }
    }

    std::vector<float>::iterator bufferFront = buffer->begin();
    std::vector<float>::iterator bufferBack;
    std::vector<float>::iterator bufferTemp;

    unsigned int sampleCount = audio.getSampleCount();
    audio.resetIterators();

    float sum;
    // for each frame (running off the end of the sample stream by delay)
    for (unsigned int inSample = 0; inSample < sampleCount + delay; inSample++) {
      // shuffle old samples along delay buffer
      bufferBack = bufferFront;
      std::advance(bufferFront, 1);
      if (bufferFront == buffer->end())
        bufferFront = buffer->begin();

      // load new sample into back of delay buffer
      if (audio.readIteratorWithinUpperBound()) {
        *bufferBack = audio.getSampleAtReadIterator() / gain;
        audio.advanceReadIterator();
      } else {
        *bufferBack = 0.0; // zero pad once we're past the end of the file
      }
      // start doing the maths once the delay has passed
      int outSample = (signed)inSample - (signed)delay;
      if (outSample < 0) continue;
      // and, if shortcut != 1, only do the maths for the useful samples (this
      // is mathematically dodgy, but it's faster and it usually works);
      if (outSample % shortcutFactor > 0) continue;
      sum = 0.0;
      bufferTemp = bufferFront;
      std::vector<float>::const_iterator coefficientIterator = coefficients.begin();
      while (coefficientIterator < coefficients.end()) {
        sum += *coefficientIterator * *bufferTemp;
        std::advance(coefficientIterator, 1);
        std::advance(bufferTemp, 1);
        if (bufferTemp == buffer->end())
          bufferTemp = buffer->begin();
      }
      audio.setSampleAtWriteIterator(sum);
      audio.advanceWriteIterator(shortcutFactor);
    }
  }

}
