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

#include "keyclassifiertest.h"

TEST(KeyClassifierTest, DetectsSilence){
  KeyFinder::KeyClassifier kc(
    KeyFinder::SIMILARITY_COSINE,
    KeyFinder::TONE_PROFILE_SHAATH,
    false
  );
  std::vector<float> chroma(12);
  ASSERT_EQ(KeyFinder::SILENCE, kc.classify(chroma));
}

TEST(KeyClassifierTest, DetectsAMinorTriad){
  std::vector<float> chromaNoOffset(12);
  chromaNoOffset[0] = 100.0; // A
  chromaNoOffset[3] = 100.0; // C
  chromaNoOffset[7] = 100.0; // E

  std::vector<float> chromaOffset(12);
  chromaOffset[9] = 100.0; // A, offset
  chromaOffset[0] = 100.0; // C, offset
  chromaOffset[4] = 100.0; // E, offset

  // No offset, cosine similarity
  KeyFinder::KeyClassifier kc1(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_KRUMHANSL, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc1.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc2(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_TEMPERLEY, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc2.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc3(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_GOMEZ, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc3.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc4(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_SHAATH, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc4.classify(chromaNoOffset));

  // No offset, correlation
  KeyFinder::KeyClassifier kc5(KeyFinder::SIMILARITY_CORRELATION, KeyFinder::TONE_PROFILE_KRUMHANSL, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc5.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc6(KeyFinder::SIMILARITY_CORRELATION, KeyFinder::TONE_PROFILE_TEMPERLEY, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc6.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc7(KeyFinder::SIMILARITY_CORRELATION, KeyFinder::TONE_PROFILE_GOMEZ, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc7.classify(chromaNoOffset));
  KeyFinder::KeyClassifier kc8(KeyFinder::SIMILARITY_CORRELATION, KeyFinder::TONE_PROFILE_SHAATH, false);
  ASSERT_EQ(KeyFinder::A_MINOR, kc8.classify(chromaNoOffset));

  // With offset, cosine similarity
  KeyFinder::KeyClassifier kc9(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_KRUMHANSL, true);
  ASSERT_EQ(KeyFinder::A_MINOR, kc9.classify(chromaOffset));
  KeyFinder::KeyClassifier kc10(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_TEMPERLEY, true);
  ASSERT_EQ(KeyFinder::A_MINOR, kc10.classify(chromaOffset));
  KeyFinder::KeyClassifier kc11(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_GOMEZ, true);
  ASSERT_EQ(KeyFinder::A_MINOR, kc11.classify(chromaOffset));
  KeyFinder::KeyClassifier kc12(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_SHAATH, true);
  ASSERT_EQ(KeyFinder::A_MINOR, kc12.classify(chromaOffset));
}

TEST(KeyClassifierTest, DetectsOtherTriads){
  // all with offset
  std::vector<float> cMajor(12);
  cMajor[0] = 100.0;
  cMajor[4] = 100.0;
  cMajor[7] = 100.0;

  std::vector<float> cMinor(12);
  cMinor[0] = 100.0;
  cMinor[3] = 100.0;
  cMinor[7] = 100.0;

  std::vector<float> gMajor(12);
  gMajor[7] = 100.0;
  gMajor[11] = 100.0;
  gMajor[2] = 100.0;

  KeyFinder::KeyClassifier kc(KeyFinder::SIMILARITY_COSINE, KeyFinder::TONE_PROFILE_SHAATH, true);
  ASSERT_EQ(KeyFinder::C_MAJOR, kc.classify(cMajor));
  ASSERT_EQ(KeyFinder::C_MINOR, kc.classify(cMinor));
  ASSERT_EQ(KeyFinder::G_MAJOR, kc.classify(gMajor));
}
