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

#include "_testhelpers.h"

static void constructToneProfile(std::vector<double> &vec) {
    KeyFinder::ToneProfile tp(vec);
}

TEST (ToneProfilesTest, ExceptionOnWrongCustomSize) {
  std::vector<double> vec(71, 0.0);
  ASSERT_THROW(constructToneProfile(vec), KeyFinder::Exception);
  std::vector<double> vec2(72, 0.0);
  ASSERT_NO_THROW(constructToneProfile(vec2));
}
/*
TEST (ToneProfilesTest, ExceptionOnWrongInputSize) {
  std::vector<double> vec(144, 0.0);
  KeyFinder::ToneProfile tp(KeyFinder::SCALE_MAJOR, vec);
  std::vector<double> vec2(73, 0.0);
  ASSERT_THROW(tp.similarity(vec2, 0), KeyFinder::Exception);
  vec2.resize(72);
  ASSERT_NO_THROW(tp.similarity(vec2, 0));
}

TEST (ToneProfilesTest, PerfectSimilarity) {
  std::vector<double> vec(144, 0.0);
  vec[0] = 1.0;
  vec[3] = 1.0;
  vec[7] = 1.0;
  KeyFinder::ToneProfile tp(KeyFinder::SCALE_MAJOR, vec);
  vec.resize(72);
  float result = tp.similarity(vec, 0);
  ASSERT_FLOAT_EQ(1.0, result);
}

TEST (ToneProfilesTest, SimilarityNormalisesMagnitude) {
  std::vector<double> vec(144, 0.0);
  vec[0] = 1.0;
  vec[3] = 1.0;
  vec[7] = 1.0;
  KeyFinder::ToneProfile tp(KeyFinder::SCALE_MAJOR, vec);
  vec.resize(72);
  vec[0] = 1000.0;
  vec[3] = 1000.0;
  vec[7] = 1000.0;
  float result = tp.similarity(vec, 0);
  ASSERT_FLOAT_EQ(1.0, result);
}

TEST (ToneProfilesTest, PerfectDissimilarity) {
  std::vector<double> vec1(144, 0.0);
  vec1[0] = 1.0;
  vec1[3] = 1.0;
  vec1[7] = 1.0;
  std::vector<double> vec2(72, 1.0);
  vec2[0] = 0.0;
  vec2[3] = 0.0;
  vec2[7] = 0.0;
  KeyFinder::ToneProfile tp(KeyFinder::SCALE_MAJOR, vec1);
  float result = tp.similarity(vec2, 0);
  ASSERT_FLOAT_EQ(0.0, result);
}

TEST (ToneProfilesTest, PartialSimilarity) {
  std::vector<double> vec1(144, 0.0);
  vec1[0] = 1.0;
  vec1[1] = 3.0;
  std::vector<double> vec2(72, 0.0);
  vec2[0] = 3.0;
  vec2[1] = 1.0;
  KeyFinder::ToneProfile tp(KeyFinder::SCALE_MAJOR, vec1);
  float result = tp.similarity(vec2, 0);
  ASSERT_FLOAT_EQ(0.6, result);
}

*/
