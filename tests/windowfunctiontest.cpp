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

TEST (WindowFunctionTest, AllTemporalWindowsAreSymmetricalAndRangeFrom0To1) {

  KeyFinder::WindowFunction win;
  unsigned int evenWidth = 24;
  unsigned int oddWidth = 25;

  for (unsigned int w = 0; w < 4; w++) {

    KeyFinder::temporal_window_t type;
    if (w % 2 == 0) type = KeyFinder::WINDOW_BLACKMAN;
    else type = KeyFinder::WINDOW_HAMMING;

    unsigned int width;
    if (w / 2 == 0) width = evenWidth;
    else width = oddWidth;

    ASSERT_NEAR(0.0, win.window(type,         0, width), 0.1);
    ASSERT_NEAR(1.0, win.window(type, width / 2, width), 0.1);
    ASSERT_NEAR(0.0, win.window(type, width - 1, width), 0.1);
    for (unsigned int n = 0; n < width / 2; n++) {
      ASSERT_FLOAT_EQ(win.window(type, n, width), win.window(type, width - 1 - n, width));
    }
  }
}

TEST (WindowFunctionTest, GaussianFn) {
  KeyFinder::WindowFunction win;
  unsigned int width = 23;
  std::vector<double> g(width, 0.0);
  for (unsigned int i = 0; i < width; i++)
    g[i] = win.gaussianWindow(i, width, sqrt(12.0));
  ASSERT_NEAR(0.0, g[0], 0.01);
  for (unsigned int i = 1; i < width / 2; i++)
    ASSERT_GT(g[i], g[i-1]);
  ASSERT_FLOAT_EQ(1.0, g[width / 2]);
  for (unsigned int i = width / 2 + 1; i < width; i++)
    ASSERT_LT(g[i], g[i-1]);
  ASSERT_NEAR(0.0, g[width - 1], 0.01);
}

TEST (WindowFunctionTest, ConvolutionOfPulseAndRectangle) {
  KeyFinder::WindowFunction win;
  unsigned int width = 101;
  std::vector<double> a(width, 0.0);
  a[width/2] = 1.0;
  std::vector<double> b(width, 1.0);
  std::vector<double> c = win.convolve(a, b);
  for (unsigned int i = 0; i < width; i++) {
    float a = c[i];
    ASSERT_FLOAT_EQ(1.0 / width, a);
  }
}

TEST (WindowFunctionTest, ConvolutionOfTwoRectangles) {
  KeyFinder::WindowFunction win;
  unsigned int width = 101;
  std::vector<double> a(width, 1.0);
  std::vector<double> b(width, 1.0);
  std::vector<double> c = win.convolve(a, b);
  ASSERT_NEAR(0.5, c[0],         0.01);
  ASSERT_NEAR(1.0, c[width / 2], 0.01);
  ASSERT_NEAR(0.5, c[width - 1], 0.01);
}

TEST (WindowFunctionTest, ConvolutionOfPulseAndCurve) {
  KeyFinder::WindowFunction win;
  unsigned int width = 101;
  std::vector<double> a(width, 0.0);
  a[width/2] = 1.0;
  std::vector<double> b(width, 0.0);
  for(unsigned int i = 0; i < width; i++)
    b[i] = win.window(KeyFinder::WINDOW_BLACKMAN, i, width);
  std::vector<double> c = win.convolve(a, b);
  for(unsigned int i = 0; i < width; i++) {
    float a = c[i];
    ASSERT_FLOAT_EQ(win.window(KeyFinder::WINDOW_BLACKMAN, i, width) / width, a);
  }
}
