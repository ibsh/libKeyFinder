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

namespace KeyFinder {

  class KeyDetectionResultSegment {
  public:
    KeyDetectionResultSegment(): firstHop(0), lastHop(0), chromaVector(0), energy(0.0), key(SILENCE) {}
    unsigned int firstHop;
    unsigned int lastHop;
    std::vector<float> chromaVector;
    float energy;
    key_t key;
  };

  class KeyDetectionResult {
  public:
    KeyDetectionResult(): segments(0), globalKeyEstimate(SILENCE) {}
    std::vector<KeyDetectionResultSegment> segments;
    key_t globalKeyEstimate;
  };

}

#endif
