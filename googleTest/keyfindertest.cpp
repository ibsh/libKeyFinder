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

#include "keyfindertest.h"

TEST (KeyFinderTest, KeyOfChromagramReturnsSilence) {
  KeyFinder::Chromagram ch(1,1,1);
  KeyFinder::KeyFinder kf;
  KeyFinder::KeyDetectionResult kdr = kf.keyOfChromagram(ch);
  ASSERT_EQ(KeyFinder::SILENCE, kdr.globalKeyEstimate);
  ASSERT_EQ(1, kdr.segments.size());
  ASSERT_EQ(12, kdr.segments[0].chromaVector.size());
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[0]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[1]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[2]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[3]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[4]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[5]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[6]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[7]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[8]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[9]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[10]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[11]);
}

TEST (KeyFinderTest, KeyOfChromagramPassesThroughChromaData) {
  KeyFinder::Chromagram ch(1,1,1);
  ch.setMagnitude(0, 0, 1.0);
  ch.setMagnitude(0, 3, 1.0);
  ch.setMagnitude(0, 7, 1.0);
  KeyFinder::KeyFinder kf;
  KeyFinder::KeyDetectionResult kdr = kf.keyOfChromagram(ch);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[0]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[1]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[2]);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[3]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[4]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[5]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[6]);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[7]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[8]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[9]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[10]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[11]);
  ASSERT_EQ(KeyFinder::C_MINOR, kdr.globalKeyEstimate);
}
