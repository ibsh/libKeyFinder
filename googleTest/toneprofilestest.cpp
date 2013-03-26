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

#include "toneprofilestest.h"

KeyFinder::tone_profile_t pr = KeyFinder::TONE_PROFILE_CUSTOM;
KeyFinder::scale_t sc        = KeyFinder::SCALE_MAJOR;

TEST (ToneProfilesTest, ExceptionOnWrongCustomSize) {
  std::vector<float> vec(23, 0.0);
  ASSERT_THROW(KeyFinder::ToneProfile tp(pr, sc, false, vec), KeyFinder::Exception);
  std::vector<float> vec2(24, 0.0);
  ASSERT_NO_THROW(KeyFinder::ToneProfile tp(pr, sc, false, vec2));
}

TEST (ToneProfilesTest, PerfectCosineSimilarity) {
  std::vector<float> vec(24, 0.0);
  vec[0] = 1.0;
  vec[3] = 1.0;
  vec[7] = 1.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec);
  float result = tp.similarity(KeyFinder::SIMILARITY_COSINE, vec, 0, 0.0);
  ASSERT_FLOAT_EQ(1.0, result);
}

TEST (ToneProfilesTest, PerfectCosineDissimilarity) {
  std::vector<float> vec1(24, 0.0);
  vec1[0] = 1.0;
  vec1[3] = 1.0;
  vec1[7] = 1.0;
  std::vector<float> vec2(24, 1.0);
  vec2[0] = 0.0;
  vec2[3] = 0.0;
  vec2[7] = 0.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec1);
  float result = tp.similarity(KeyFinder::SIMILARITY_COSINE, vec2, 0, 0.0);
  ASSERT_FLOAT_EQ(0.0, result);
}

// TODO: Less extreme cosine tests
// TODO: Correlation tests
