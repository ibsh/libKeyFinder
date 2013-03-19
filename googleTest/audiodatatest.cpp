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

#include "audiodatatest.h"

TEST (AudioDataTest, ConstructorWorks) {
  KeyFinder::AudioData a;
  ASSERT_EQ(0, a.getChannels());
  ASSERT_EQ(0, a.getFrameRate());
  ASSERT_EQ(0, a.getSampleCount());
}

TEST (AudioDataTest, Channels) {
  KeyFinder::AudioData a;
  a.setChannels(2);
  ASSERT_EQ(2, a.getChannels());
  ASSERT_THROW(a.setChannels(0), KeyFinder::Exception);
}

TEST (AudioDataTest, FrameRate) {
  KeyFinder::AudioData a;
  a.setFrameRate(44100);
  ASSERT_EQ(44100, a.getFrameRate());
  ASSERT_THROW(a.setFrameRate(0), KeyFinder::Exception);
}

TEST (AudioDataTest, SamplesBasic) {
  KeyFinder::AudioData a;
  a.addToSampleCount(100);
  ASSERT_EQ(100, a.getSampleCount());
  // init values
  for (int i = 0; i < 100; i++) {
    ASSERT_FLOAT_EQ(0.0, a.getSample(i));
  }
  a.setSample(0, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(0));
}

TEST (AudioDataTest, Bounds) {
  KeyFinder::AudioData a;
  a.addToSampleCount(5);
  ASSERT_THROW(a.getSample(-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample(5), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(-1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(5, 1.0), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(0, NAN), KeyFinder::Exception);
}

TEST (AudioDataTest, MakeMono) {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.addToSampleCount(20);
  for (int i = 0; i < 10; i++) {
    a.setSample(i * 2, 100.0);
  }
  a.reduceToMono();
  ASSERT_EQ(10, a.getSampleCount());
  for (int i = 0; i < 10; i++) {
    ASSERT_FLOAT_EQ(50.0, a.getSample(i));
  }
}

TEST (AudioDataTest, Frames) {
  KeyFinder::AudioData a;
  a.addToSampleCount(4);
  ASSERT_THROW(a.getSample(0,0), KeyFinder::Exception);

  a.setChannels(2);
  ASSERT_FLOAT_EQ(0.0, a.getSample(0, 0));
  ASSERT_FLOAT_EQ(0.0, a.getSample(0, 1));

  ASSERT_THROW(a.getSample(-1, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSample(-1, 1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 2, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 2, 1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 0,-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 0, 2), KeyFinder::Exception);

  a.setSample(0, 0, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(0,0));

  a.setSample(1, 0, 20.0);
  ASSERT_FLOAT_EQ(20.0, a.getSample(2));
}
