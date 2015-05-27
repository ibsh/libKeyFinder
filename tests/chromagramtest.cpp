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

TEST (ChromagramTest, ConstructorDefaultsWork) {
  KeyFinder::Chromagram c;
  ASSERT_EQ(0, c.getHops());
}

TEST (ChromagramTest, ConstructorArgumentsWork) {
  KeyFinder::Chromagram c(20);
  ASSERT_EQ(20, c.getHops());
  for (int h = 0; h < 20; h++) {
    for (int b = 0; b < BANDS; b++) {
      ASSERT_FLOAT_EQ(0.0, c.getMagnitude(h, b));
    }
  }
}

TEST (ChromagramTest, Mutator) {
  KeyFinder::Chromagram c(1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
}

TEST (ChromagramTest, CopyConstructor) {
  KeyFinder::Chromagram c(1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_EQ(1, c.getHops());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c.getMagnitude(0, 1));

  KeyFinder::Chromagram c2(c);
  ASSERT_EQ(1, c2.getHops());
  ASSERT_FLOAT_EQ(1.0, c2.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c2.getMagnitude(0, 1));
}

TEST (ChromagramTest, Assignment) {
  KeyFinder::Chromagram c(1);
  c.setMagnitude(0, 0, 1.0);
  ASSERT_EQ(1, c.getHops());
  ASSERT_FLOAT_EQ(1.0, c.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c.getMagnitude(0, 1));

  KeyFinder::Chromagram c2 = c;
  ASSERT_EQ(1, c2.getHops());
  ASSERT_FLOAT_EQ(1.0, c2.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(0.0, c2.getMagnitude(0, 1));
}

TEST (ChromagramTest, Bounds) {
  KeyFinder::Chromagram c(5);
  // hops min max
  ASSERT_THROW(c.getMagnitude(-1,  0), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 5,  0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude(-1,  0, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 5,  0, 1.0), KeyFinder::Exception);
  // bands min max
  ASSERT_THROW(c.getMagnitude( 0, -1), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 0, BANDS), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, -1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, BANDS, 1.0), KeyFinder::Exception);
  // value bounds
  ASSERT_THROW(c.setMagnitude( 0,  0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0,  0, NAN), KeyFinder::Exception);
}

TEST (ChromagramTest, Append) {
  KeyFinder::Chromagram a(1);
  KeyFinder::Chromagram b(1);
  a.setMagnitude(0, 0, 10.0);
  b.setMagnitude(0, 0, 20.0);
  ASSERT_NO_THROW(a.append(b));
  ASSERT_EQ(2, a.getHops());
  ASSERT_FLOAT_EQ(10.0, a.getMagnitude(0, 0));
  ASSERT_FLOAT_EQ(20.0, a.getMagnitude(1, 0));
}

TEST (ChromagramTest, CollapseToOneHop) {

  KeyFinder::Chromagram c(3);
  c.setMagnitude(0, 0, 10.0);
  c.setMagnitude(1, 0, 15.0);
  c.setMagnitude(2, 0, 20.0);

  std::vector<double> d = c.collapseToOneHop();

  ASSERT_EQ(72, d.size());
  ASSERT_FLOAT_EQ(15.0, d[0]);
}
