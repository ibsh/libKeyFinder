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

#include "windowfunctiontest.h"

TEST(WindowFunctionTest, GaussianFn){
  KeyFinder::WindowFunction win;
  unsigned int width = 23;
  std::vector<float> g(width, 0.0);
  for(unsigned int i = 0; i < width; i++)
    g[i] = win.gaussianWindow(i, width, sqrt(12.0));
  ASSERT_NEAR(0.0, g[0], 0.01);
  for(unsigned int i = 1; i < width / 2; i++)
    ASSERT_GT(g[i], g[i-1]);
  ASSERT_FLOAT_EQ(1.0, g[width / 2]);
  for(unsigned int i = width / 2 + 1; i < width; i++)
    ASSERT_LT(g[i], g[i-1]);
  ASSERT_NEAR(0.0, g[width - 1], 0.01);
}
