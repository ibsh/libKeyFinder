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

TEST(SegmentationTest, NoSegmentationWorks){
  KeyFinder::Parameters p;
  p.setSegmentation(KeyFinder::SEGMENTATION_NONE);

  KeyFinder::Chromagram c(10,1,1);
  KeyFinder::Segmentation seg;
  std::vector<unsigned int> sb = seg.getSegmentationBoundaries(c, p);
  ASSERT_EQ(1, sb.size());
  ASSERT_EQ(0, sb[0]);
}

TEST(SegmentationTest, ArbitrarySegmentationWorks){
  KeyFinder::Parameters p;
  p.setSegmentation(KeyFinder::SEGMENTATION_ARBITRARY);
  p.setArbitrarySegments(3);

  KeyFinder::Chromagram c(21,1,1);
  KeyFinder::Segmentation seg;
  std::vector<unsigned int> sb = seg.getSegmentationBoundaries(c, p);
  ASSERT_EQ(3, sb.size());
  ASSERT_EQ( 0, sb[0]);
  ASSERT_EQ( 7, sb[1]);
  ASSERT_EQ(14, sb[2]);
}
