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

#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <cmath>
#include "gtest/gtest.h"

#undef  PI
#define PI 3.1415926535897932384626433832795

float sine_wave (
  unsigned int index,
  float frequency,
  unsigned int sampleRate,
  unsigned int magnitude = 1
);

#endif // TESTHELPERS_H
