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

#ifndef KEYDETECTIONRESULT_H
#define KEYDETECTIONRESULT_H

#include "chromagram.h"

namespace KeyFinder{

  class KeyDetectionSegment{
  public:
    KeyDetectionSegment(): key(SILENCE), firstHop(0), lastHop(0), energy(0.0){}
    key_t key;
    unsigned int firstHop;
    unsigned int lastHop;
    float energy;
  };

  class KeyDetectionResult{
  public:
    Chromagram fullChromagram;
    Chromagram oneOctaveChromagram;
    std::vector<float> harmonicChangeSignal;
    std::vector<KeyDetectionSegment> segments;
    key_t globalKeyEstimate;
  };

}

#endif
