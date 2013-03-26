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

#include "keyfinderresulttest.h"

TEST (KeyFinderResultTest, ConstructorDefaultsWork) {
  KeyFinder::KeyDetectionResultSegment kdrs;
  ASSERT_EQ(0, kdrs.firstHop);
  ASSERT_EQ(0, kdrs.lastHop);
  ASSERT_EQ(0, kdrs.chromaVector.size());
  ASSERT_FLOAT_EQ(0.0, kdrs.energy);
  ASSERT_EQ(KeyFinder::SILENCE, kdrs.key);

  KeyFinder::KeyDetectionResult kdr;
  ASSERT_EQ(0, kdr.segments.size());
  ASSERT_EQ(KeyFinder::SILENCE, kdr.globalKeyEstimate);
}
