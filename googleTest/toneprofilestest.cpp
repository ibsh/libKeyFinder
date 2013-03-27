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

KeyFinder::tone_profile_t pr       = KeyFinder::TONE_PROFILE_CUSTOM;
KeyFinder::scale_t sc              = KeyFinder::SCALE_MAJOR;
KeyFinder::similarity_measure_t cs = KeyFinder::SIMILARITY_COSINE;
KeyFinder::similarity_measure_t cr = KeyFinder::SIMILARITY_CORRELATION;

TEST (ToneProfilesTest, ExceptionOnWrongCustomSize) {
  std::vector<float> vec(23, 0.0);
  ASSERT_THROW(KeyFinder::ToneProfile tp(pr, sc, false, vec), KeyFinder::Exception);
  std::vector<float> vec2(24, 0.0);
  ASSERT_NO_THROW(KeyFinder::ToneProfile tp(pr, sc, false, vec2));
}

TEST (ToneProfilesTest, ExceptionOnWrongInputSize) {
  std::vector<float> vec(24, 0.0);
  KeyFinder::ToneProfile tp(pr, sc, false, vec);
  std::vector<float> vec2(13, 0.0);
  ASSERT_THROW(tp.similarity(cs, vec2, 0), KeyFinder::Exception);
  vec2.resize(12);
  ASSERT_NO_THROW(tp.similarity(cs, vec2, 0));
}

TEST (ToneProfilesTest, PerfectSimilarity) {
  std::vector<float> vec(24, 0.0);
  vec[0] = 1.0;
  vec[3] = 1.0;
  vec[7] = 1.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec);
  vec.resize(12);
  float rslt_cos = tp.similarity(cs, vec, 0);
  ASSERT_FLOAT_EQ(1.0, rslt_cos);
  float rslt_cor = tp.similarity(cr, vec, 0);
  ASSERT_FLOAT_EQ(1.0, rslt_cor);
}

TEST (ToneProfilesTest, SimilarityNormalisesMagnitude) {
  std::vector<float> vec(24, 0.0);
  vec[0] = 1.0;
  vec[3] = 1.0;
  vec[7] = 1.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec);
  vec.resize(12);
  vec[0] = 1000.0;
  vec[3] = 1000.0;
  vec[7] = 1000.0;
  float rslt_cos = tp.similarity(cs, vec, 0);
  ASSERT_FLOAT_EQ(1.0, rslt_cos);
  float rslt_cor = tp.similarity(cr, vec, 0);
  ASSERT_FLOAT_EQ(1.0, rslt_cor);
}

TEST (ToneProfilesTest, PerfectDissimilarity) {
  std::vector<float> vec1(24, 0.0);
  vec1[0] = 1.0;
  vec1[3] = 1.0;
  vec1[7] = 1.0;
  std::vector<float> vec2(12, 1.0);
  vec2[0] = 0.0;
  vec2[3] = 0.0;
  vec2[7] = 0.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec1);
  float rslt_cos = tp.similarity(cs, vec2, 0);
  ASSERT_FLOAT_EQ(0.0, rslt_cos);
  float rslt_cor = tp.similarity(cr, vec2, 0);
  ASSERT_FLOAT_EQ(-1.0, rslt_cor);
}

TEST (ToneProfilesTest, PartialSimilarity) {
  std::vector<float> vec1(24, 0.0);
  vec1[0] = 1.0;
  vec1[1] = 3.0;
  std::vector<float> vec2(12, 0.0);
  vec2[0] = 3.0;
  vec2[1] = 1.0;
  KeyFinder::ToneProfile tp(pr, sc, false, vec1);
  float rslt_cos = tp.similarity(cs, vec2, 0);
  ASSERT_FLOAT_EQ(0.6, rslt_cos);
  float rslt_cor = tp.similarity(cr, vec2, 0);
  ASSERT_NEAR(0.54, rslt_cor, 0.005);
}

