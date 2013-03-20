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

#ifndef LOWPASSFILTERTEST_H
#define LOWPASSFILTERTEST_H

#include "_testhelpers.h"
#include "keyfinder/lowpassfilter.h"

class DefaultLowPassFilterWithPublicCoefficients : public KeyFinder::LowPassFilter {
public:
  DefaultLowPassFilterWithPublicCoefficients() : KeyFinder::LowPassFilter(160, 44100, 2000.0, 2048) {}
  std::vector<float> getCoefficients() const;
};

class LowPassFilterTest : public ::testing::Test { };

#endif // LOWPASSFILTERTEST_H
