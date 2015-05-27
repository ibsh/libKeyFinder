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

TEST (TemporalWindowFactoryTest, FrameSize) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<double>* tw1 = twf.getTemporalWindow(10);
  ASSERT_EQ(10, tw1->size());
}

TEST (TemporalWindowFactoryTest, Function) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<double>* tw1 = twf.getTemporalWindow(1000);

  KeyFinder::WindowFunction win;
  for (unsigned int i = 0; i < 1000; i++) {
    float a = win.window(KeyFinder::WINDOW_BLACKMAN, i, 1000);
    float b = tw1->at(i);
    ASSERT_FLOAT_EQ(a, b);
  }
}

TEST (TemporalWindowFactoryTest, RepeatedWindowRequests) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<double>* tw1 = twf.getTemporalWindow(10);
  const std::vector<double>* tw2 = twf.getTemporalWindow(10);
  const std::vector<double>* tw3 = twf.getTemporalWindow(12);

  ASSERT_EQ(tw1, tw2);
  ASSERT_NE(tw2, tw3);
}
