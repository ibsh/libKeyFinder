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

#include "segnone.h"

namespace KeyFinder{

  std::vector<float> NoSeg::getRateOfChange(const Chromagram& ch, const Parameters& /*params*/){
    std::vector<float> NoChange(ch.getHops());
    return NoChange;
  }

  std::vector<unsigned int> NoSeg::getSegments(const std::vector<float>& /*rateOfChange*/, const Parameters& /*params*/){
    std::vector<unsigned int> noChanges(1, 0);
    return noChanges;
  }

}
