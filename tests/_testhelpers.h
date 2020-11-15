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

#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <cmath>
#include "catch/catch.hpp"
#include "keyfinder.h"

// all of this is a bit weak.
#define ASSERT(expr) REQUIRE(expr)
#define ASSERT_TRUE(expr) REQUIRE(expr)
#define ASSERT_FALSE(expr) REQUIRE_FALSE(expr)
#define ASSERT_EQ(a,b) REQUIRE((a) == (b))
#define ASSERT_NE(a,b) REQUIRE((a) != (b))
#define ASSERT_GT(a,b) REQUIRE((a) >  (b))
#define ASSERT_GE(a,b) REQUIRE((a) >= (b))
#define ASSERT_LT(a,b) REQUIRE((a) <  (b))
#define ASSERT_LE(a,b) REQUIRE((a) <= (b))

// this is shit. find GTest's macro maybe.
#define TINY 0.0000001
#define ASSERT_FLOAT_EQ(a,b) REQUIRE((a) >= (b) - TINY); REQUIRE((a) <= (b) + TINY)
#define ASSERT_NEAR(a,b,d) REQUIRE((a) >= (b) - (d)); REQUIRE((a) <= (b) + (d))

// just fix.
#define ASSERT_THROW(expr, exc_type) REQUIRE_THROWS_AS(expr, exc_type)
#define ASSERT_NO_THROW(expr) REQUIRE_NOTHROW(expr)

// just to keep using GTest's TEST() macro; use a text editor instead and fix.
#define STRINGIFY_ULTRA(s) #s
#define STRINGIFY(s) STRINGIFY_ULTRA(s)
#define TEST(a,b) TEST_CASE(STRINGIFY(a) "/" STRINGIFY(b))

float sine_wave (
  unsigned int index,
  float frequency,
  unsigned int sampleRate,
  unsigned int magnitude = 1
);

#endif // TESTHELPERS_H
