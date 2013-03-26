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

#include "binodetest.h"

TEST (BinodeTest, ConstructorDefaultsWork) {
  KeyFinder::Binode<float> bf;
  ASSERT_FLOAT_EQ(0.0, bf.data);
  ASSERT_EQ(NULL, bf.l);
  ASSERT_EQ(NULL, bf.r);

  KeyFinder::Binode<int> bi;
  ASSERT_EQ(0, bi.data);
  ASSERT_EQ(NULL, bi.l);
  ASSERT_EQ(NULL, bi.r);
}

TEST (BinodeTest, ConstructorArgumentsWork) {
  KeyFinder::Binode<float> bf(365.25);
  ASSERT_FLOAT_EQ(365.25, bf.data);
  ASSERT_EQ(NULL, bf.l);
  ASSERT_EQ(NULL, bf.r);

  KeyFinder::Binode<int> bi(14);
  ASSERT_EQ(14, bi.data);
}
