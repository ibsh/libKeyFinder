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

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "constants.h"

namespace KeyFinder {

  class AudioData {
  public:
    AudioData();

    unsigned int getChannels() const;
    unsigned int getFrameRate() const;
    double getSample(unsigned int index) const;
    double getSampleByFrame(unsigned int frame, unsigned int channel) const;
    double getSampleAtReadIterator() const;
    unsigned int getSampleCount() const;
    unsigned int getFrameCount() const;

    void setChannels(unsigned int newChannels);
    void setFrameRate(unsigned int newFrameRate);
    void setSample(unsigned int index, double value);
    void setSampleByFrame(unsigned int frame, unsigned int channels, double value);
    void setSampleAtWriteIterator(double value);
    void addToSampleCount(unsigned int newSamples);
    void addToFrameCount(unsigned int newFrames);

    void advanceReadIterator(unsigned int by = 1);
    void advanceWriteIterator(unsigned int by = 1);
    bool readIteratorWithinUpperBound() const;
    bool writeIteratorWithinUpperBound() const;
    void resetIterators();

    void append(const AudioData& that);
    void prepend(const AudioData& that);
    void discardFramesFromFront(unsigned int discardFrameCount);
    void reduceToMono();
    void downsample(unsigned int factor, bool shortcut = true);
    AudioData* sliceSamplesFromBack(unsigned int sliceSampleCount);

  private:
    std::deque<double> samples;
    unsigned int channels;
    unsigned int frameRate;
    std::deque<double>::const_iterator readIterator;
    std::deque<double>::iterator writeIterator;
  };

}

#endif
