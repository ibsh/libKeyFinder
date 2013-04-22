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

#include "downsampler.h"


namespace KeyFinder {

  void Downsampler::downsample(AudioData& audioIn, unsigned int factor) const {
    if (factor == 1) return;

    unsigned int channels = audioIn.getChannels();
    unsigned int oldFrameCount = audioIn.getFrameCount();
    unsigned int newFrameCount = ceil((float)oldFrameCount / (float)factor);

    AudioData audioOut;
    audioOut.setFrameRate(audioIn.getFrameRate() / factor);
    audioOut.setChannels(channels);
    audioOut.addToFrameCount(newFrameCount);

    // for each frame of the output
    for (unsigned int outFrm = 0; outFrm < newFrameCount; outFrm++) {
      // for each channel
      for (unsigned int ch = 0; ch < channels; ch++) {
        // take the mean of a set of input frames
        float mean = 0.0;
        for (unsigned int element = 0; element < factor; element++) {
          unsigned int inFrm = (outFrm * factor) + element;
          if (inFrm < audioIn.getFrameCount()) {
            mean += audioIn.getSample(inFrm, ch) / (float)factor;
          }
        }
        audioOut.setSample(outFrm, ch, mean);
      }
    }

    audioIn = audioOut;
  }

}
