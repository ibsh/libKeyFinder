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

#include "segmentationtest.h"

TEST (SegmentationTest, NoSegmentationWorks) {
  KeyFinder::Parameters p;
  p.setSegmentation(KeyFinder::SEGMENTATION_NONE);

  KeyFinder::Chromagram c(10, 1, 1);
  KeyFinder::Segmentation seg;
  std::vector<unsigned int> sb = seg.getSegmentationBoundaries(c, p);
  ASSERT_EQ(1, sb.size());
  ASSERT_EQ(0, sb[0]);
}

TEST (SegmentationTest, ArbitrarySegmentationWorks) {
  KeyFinder::Parameters p;
  p.setSegmentation(KeyFinder::SEGMENTATION_ARBITRARY);
  p.setArbitrarySegments(3);

  KeyFinder::Chromagram c(21, 1, 1);
  KeyFinder::Segmentation seg;
  std::vector<unsigned int> sb = seg.getSegmentationBoundaries(c, p);
  ASSERT_EQ(3, sb.size());
  ASSERT_EQ( 0, sb[0]);
  ASSERT_EQ( 7, sb[1]);
  ASSERT_EQ(14, sb[2]);
}

TEST (SegmentationTest, ChangeDetectionSegmentationWorks) {
  KeyFinder::Parameters p;
  p.setSegmentation(KeyFinder::SEGMENTATION_COSINE);
  p.setSegGaussianSize(11);
  p.setSegGaussianSigma(2.0);

  // changes: silent > c minor > db major > g minor > silent
  KeyFinder::Chromagram c(200, 1, 1);
  for (unsigned int i = 40; i < 80; i++) {
    c.setMagnitude(i, 0, 1.0);
    c.setMagnitude(i, 3, 1.0);
    c.setMagnitude(i, 7, 1.0);
  }
  for (unsigned int i = 80; i < 120; i++) {
    c.setMagnitude(i, 3, 1.0);
    c.setMagnitude(i, 7, 1.0);
    c.setMagnitude(i, 10, 1.0);
  }
  for (unsigned int i = 120; i < 160; i++) {
    c.setMagnitude(i,  7, 1.0);
    c.setMagnitude(i, 10, 1.0);
    c.setMagnitude(i,  2, 1.0);
  }
  KeyFinder::Segmentation seg;
  std::vector<unsigned int> sb = seg.getSegmentationBoundaries(c, p);
  ASSERT_EQ(5, sb.size());
  ASSERT_EQ(  0, sb[0]);
  ASSERT_EQ( 40, sb[1]);
  ASSERT_EQ( 80, sb[2]);
  ASSERT_EQ(120, sb[3]);
  ASSERT_EQ(160, sb[4]);
}
