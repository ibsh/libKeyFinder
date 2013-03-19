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

#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#include <boost/math/special_functions/fpclassify.hpp>
#include <vector>
#include <cmath>
#include "parameters.h"

namespace KeyFinder {

  class Chromagram {
  public:
    Chromagram(unsigned int hops = 0, unsigned int octaves = 0, unsigned int bandsPerSemitone = 0);
    void append(const Chromagram& that);
    void setMagnitude(unsigned int hop, unsigned int band, float value);
    float getMagnitude(unsigned int hop, unsigned int band) const;
    unsigned int getHops() const;
    unsigned int getBands() const;
    unsigned int getBandsPerSemitone() const;
    unsigned int getOctaves() const;
    void reduceToOneOctave();
    void tuningHarte();
    void tuningBandAdaptive(float detunedBandWeight);
  private:
    unsigned int bandsPerSemitone;
    unsigned int octaves;
    std::vector< std::vector<float> > chromaData;
  };

}

#endif
