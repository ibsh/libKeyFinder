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

#include "temporalwindowfactorytest.h"

TEST (TemporalWindowFactoryTest, FrameSize) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<float>* tw1 = twf.getTemporalWindow(10, KeyFinder::WINDOW_BLACKMAN);
  ASSERT_EQ(10, tw1->size());
}

TEST (TemporalWindowFactoryTest, Function) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<float>* tw1 = twf.getTemporalWindow(1000, KeyFinder::WINDOW_BLACKMAN);

  KeyFinder::WindowFunction win;
  for (unsigned int i = 0; i < 1000; i++)
    ASSERT_FLOAT_EQ(win.window(KeyFinder::WINDOW_BLACKMAN, i, 1000), tw1->at(i));
}

TEST (TemporalWindowFactoryTest, RepeatedWindowRequests) {
  KeyFinder::TemporalWindowFactory twf;

  const std::vector<float>* tw1 = twf.getTemporalWindow(10, KeyFinder::WINDOW_BLACKMAN);
  const std::vector<float>* tw2 = twf.getTemporalWindow(10, KeyFinder::WINDOW_BLACKMAN);
  const std::vector<float>* tw3 = twf.getTemporalWindow(12, KeyFinder::WINDOW_BLACKMAN);
  const std::vector<float>* tw4 = twf.getTemporalWindow(12, KeyFinder::WINDOW_BLACKMAN);
  const std::vector<float>* tw5 = twf.getTemporalWindow(12, KeyFinder::WINDOW_HAMMING);

  ASSERT_EQ(tw1, tw2);
  ASSERT_NE(tw2, tw3);
  ASSERT_EQ(tw3, tw4);
  ASSERT_NE(tw4, tw5);
}
