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

#include "chromagramtest.h"

TEST(ChromagramTest, ConstructorDefaultsWork){
  KeyFinder::Chromagram c;
  ASSERT_EQ(0, c.getHops());
  ASSERT_EQ(0, c.getBins());
}

TEST(ChromagramTest, ConstructorArgumentsWork){
  KeyFinder::Chromagram c(20, 12);
  ASSERT_EQ(20, c.getHops());
  ASSERT_EQ(12, c.getBins());
  // init values
  for(int h=0; h<20; h++){
    for(int b=0; b<12; b++){
      ASSERT_EQ(0.0, c.getMagnitude(h, b));
    }
  }
}

TEST(ChromagramTest, Mutator){
  KeyFinder::Chromagram c(1, 1);
  c.setMagnitude(0,0, 100.0);
  ASSERT_EQ(100.0, c.getMagnitude(0, 0));
}

TEST(ChromagramTest, Bounds){
  KeyFinder::Chromagram c(5, 12);
  // hops min max
  ASSERT_THROW(c.getMagnitude(-1,  0), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 5,  0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude(-1,  0, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 5,  0, 1.0), KeyFinder::Exception);
  // bins min max
  ASSERT_THROW(c.getMagnitude( 0, -1), KeyFinder::Exception);
  ASSERT_THROW(c.getMagnitude( 0, 12), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, -1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0, 12, 1.0), KeyFinder::Exception);
  // value bounds
  ASSERT_THROW(c.setMagnitude( 0,  0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(c.setMagnitude( 0,  0, NAN), KeyFinder::Exception);
}

TEST(ChromagramTest, NilTuning){
  KeyFinder::Parameters p;
  p.setOctaves(1);
  KeyFinder::Chromagram c(1, 12);

  p.setTuningMethod(KeyFinder::TUNING_BIN_ADAPTIVE);
  c.reduceTuningBins(p);
  ASSERT_EQ(12, c.getBins());

  p.setTuningMethod(KeyFinder::TUNING_HARTE);
  c.reduceTuningBins(p);
  ASSERT_EQ(12, c.getBins());
}

TEST(ChromagramTest, AdaptiveTuning){
  KeyFinder::Parameters p;
  p.setOctaves(1);
  p.setBandsPerSemitone(3);
  p.setTuningMethod(KeyFinder::TUNING_BIN_ADAPTIVE);
  p.setDetunedBandWeight(0.1);

  KeyFinder::Chromagram c(1, 36);
  c.setMagnitude(0, 0, 100.0);
  c.setMagnitude(0, 1,  10.0);
  c.setMagnitude(0, 2,   0.0);

  c.setMagnitude(0, 3,   0.0);
  c.setMagnitude(0, 4,  10.0);
  c.setMagnitude(0, 5, 100.0);

  c.setMagnitude(0, 6,  10.0);
  c.setMagnitude(0, 7, 100.0);
  c.setMagnitude(0, 8,  10.0);

  c.reduceTuningBins(p);
  ASSERT_EQ(12, c.getBins());
  ASSERT_EQ(101.0, c.getMagnitude(0,0));
  ASSERT_EQ(101.0, c.getMagnitude(0,1));
  ASSERT_EQ(102.0, c.getMagnitude(0,2));
}

//TEST(ChromagramTest, HarteTuning){
//  FAIL();
//}
