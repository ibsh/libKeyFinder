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

#include "lowpassfilterfactorytest.h"

TEST (LowPassFilterFactoryTest, RepeatedFilterRequests) {
  KeyFinder::LowPassFilterFactory lpff;

  KeyFinder::LowPassFilter* lpf1 = lpff.getLowPassFilter(2, 1, 20.0, 8);
  KeyFinder::LowPassFilter* lpf2 = lpff.getLowPassFilter(2, 1, 20.0, 8);
  KeyFinder::LowPassFilter* lpf3 = lpff.getLowPassFilter(2, 1, 20.0, 16);

  ASSERT_EQ(lpf1, lpf2);
  ASSERT_NE(lpf2, lpf3);
}
