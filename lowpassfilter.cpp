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
    if (ord %2 != 0) throw Exception("LPF order must be an even number");
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
      if (i / (float) fftFrameSize <= cutoffPoint) {
        input = tau;
      }
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

  void LowPassFilter::filter(AudioData& audioIn, unsigned int shortcutFactor) const {

    // create circular delay buffer
    // this must be done in here for thread safety
    Binode<float>* p = new Binode<float>(); // first node
    Binode<float>* q = p;
    for (unsigned int i=0; i<order; i++) {
      q->r = new Binode<float>(); // subsequent nodes, for a total of impulseLength
      q->r->l = q;
      q = q->r;
    }
    // join first and last nodes
    p->l = q;
    q->r = p;

    unsigned int frameCount = audioIn.getFrameCount();
    unsigned int channels = audioIn.getChannels();

    // prep output stream
    AudioData audioOut;
    audioOut.setFrameRate(audioIn.getFrameRate());
    audioOut.setChannels(channels);
    audioOut.addToFrameCount(frameCount);

    // for each channel (should be mono by this point but just in case)
    for (unsigned int ch = 0; ch < channels; ch++) {
      Binode<float>* q = p;
      // clear delay buffer
      for (unsigned int k = 0; k < impulseLength; k++) {
        q->data = 0.0;
        q = q->r;
      }
      // for each frame (running off the end of the sample stream by delay)
      for (unsigned int frm = 0; frm < frameCount + delay; frm++) {

        // shuffle old samples along delay buffer
        p = p->r;

        // load new sample into delay buffer
        if (frm < frameCount) {
          p->l->data = audioIn.getSample(frm, ch) / gain;
        } else {
          // zero pad once we're into the delay at the end of the file
          p->l->data = 0.0;
        }

        // start doing the maths once the delay has passed
        // and, if shortcut != 1, only do the maths for the useful samples
        // (this is mathematically dodgy, but it's fast and it usually works)
        if ((frm >= delay) && (frm - delay) % shortcutFactor == 0) {
          float sum = 0.0;
          q = p;
          for (unsigned int k = 0; k < impulseLength; k++) {
            sum += coefficients[k] * q->data;
            q = q->r;
          }
          audioOut.setSample(frm - delay, ch, sum);
        }
      }
    }

    // delete delay buffer
    for (unsigned int i = 0; i < impulseLength; i++) {
      q = p;
      p = p->r;
      delete q;
    }

    audioIn = audioOut ;

  }

}
