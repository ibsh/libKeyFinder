/*************************************************************************

  Copyright 2012 Ibrahim Sha'ath

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

#include "parameters.h"
#include <vector>
#include <math.h>
#include <stdlib.h>

namespace KeyFinder{

  class Chromagram{
  public:
    Chromagram(unsigned int h = 0, unsigned int b = 0);
    Chromagram(const Chromagram&);
    void setMagnitude(unsigned int, unsigned int, float);
    float getMagnitude(unsigned int, unsigned int) const;
    unsigned int getHops() const;
    unsigned int getBins() const;
    void reduceToOneOctave(const Parameters&);
    void reduceTuningBins(const Parameters&);
  private:
    unsigned int hops;
    unsigned int bins;
    std::vector<std::vector<float> > chromaData;
    void tuningHarte(const Parameters&);
    void tuningBinAdaptive(const Parameters&);
  };

} // namespace

#endif
