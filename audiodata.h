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

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <boost/math/special_functions/fpclassify.hpp>
#include <vector>
#include "exception.h"

namespace KeyFinder{

  class AudioData{
  public:
    AudioData();

    unsigned int getChannels() const;
    unsigned int getFrameRate() const;
    float getSample(unsigned int) const;
    float getSample(unsigned int, unsigned int) const;
    unsigned int getSampleCount() const;
    unsigned int getFrameCount() const;

    void setChannels(unsigned int);
    void setFrameRate(unsigned int);
    void setSample(unsigned int, float);
    void setSample(unsigned int, unsigned int, float);
    void addToSampleCount(unsigned int);
    void addToFrameCount(unsigned int);

    void reduceToMono();

  private:
    std::vector<float> samples;
    unsigned int channels;
    unsigned int frameRate;
    unsigned int sampleCount;
  };

}

#endif
