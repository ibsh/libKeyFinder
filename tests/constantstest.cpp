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

TEST (ConstantsTest, aFewDefaultBandFreqs) {
  ASSERT_NEAR(32.7,    KeyFinder::getFrequencyOfBand(0), 0.01);
  ASSERT_NEAR(55.0,    KeyFinder::getFrequencyOfBand(9), 0.01);
  ASSERT_NEAR(1975.53, KeyFinder::getLastFrequency(),  0.01);
}

TEST (ConstantsTest, FreqBounds) {
  ASSERT_THROW(KeyFinder::getFrequencyOfBand(-1), KeyFinder::Exception);
  ASSERT_NO_THROW(KeyFinder::getFrequencyOfBand(71));
  ASSERT_THROW(KeyFinder::getFrequencyOfBand(72), KeyFinder::Exception);
}
