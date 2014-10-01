/*************************************************************************

  Copyright 2011-2014 Ibrahim Sha'ath

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

#include "chromagram.h"

namespace KeyFinder {

  Chromagram::Chromagram(unsigned int hops) :
    chromaData(hops, std::vector<float>(BANDS, 0.0))
  { }

  float Chromagram::getMagnitude(unsigned int hop, unsigned int band) const {
    if (hop >= getHops()) {
      std::ostringstream ss;
      ss << "Cannot get magnitude of out-of-bounds hop (" << hop << "/" << getHops() << ")";
      throw Exception(ss.str().c_str());
    }
    if (band >= BANDS) {
      std::ostringstream ss;
      ss << "Cannot get magnitude of out-of-bounds band (" << band << "/" << BANDS << ")";
      throw Exception(ss.str().c_str());
    }
    return chromaData[hop][band];
  }

  void Chromagram::setMagnitude(unsigned int hop, unsigned int band, float value) {
    if (hop >= getHops()) {
      std::ostringstream ss;
      ss << "Cannot set magnitude of out-of-bounds hop (" << hop << "/" << getHops() << ")";
      throw Exception(ss.str().c_str());
    }
    if (band >= BANDS) {
      std::ostringstream ss;
      ss << "Cannot set magnitude of out-of-bounds band (" << band << "/" << BANDS << ")";
      throw Exception(ss.str().c_str());
    }
    if (!std::isfinite(value)) {
      throw Exception("Cannot set magnitude to NaN");
    }
    chromaData[hop][band] = value;
  }

  std::vector<float> Chromagram::collapseToOneHop() const {
    std::vector<float> oneHop = std::vector<float>(BANDS, 0.0);
    for (unsigned int h = 0; h < getHops(); h++) {
        for (unsigned int b = 0; b < BANDS; b++) {
            oneHop[b] += getMagnitude(h, b) / getHops();
        }
    }
    return oneHop;
  }

  void Chromagram::append(const Chromagram& that) {
    chromaData.insert(chromaData.end(), that.chromaData.begin(), that.chromaData.end());
  }

  unsigned int Chromagram::getHops() const {
    return chromaData.size();
  }

}
