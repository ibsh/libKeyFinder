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

TEST (AudioDataTest, SampleInitialisation) {
  KeyFinder::AudioData a;
  a.addToSampleCount(100);
  ASSERT_EQ(100, a.getSampleCount());
  // init values
  for (int i = 0; i < 100; i++) {
    ASSERT_FLOAT_EQ(0.0, a.getSample(i));
  }
}

TEST (AudioDataTest, SampleMutator) {
  KeyFinder::AudioData a;
  a.addToSampleCount(1);
  a.setSample(0, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(0));
}

TEST (AudioDataTest, SampleMutatorBounds) {
  KeyFinder::AudioData a;
  a.addToSampleCount(5);
  ASSERT_THROW(a.getSample(-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample(5), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(-1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(5, 1.0), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(0, NAN), KeyFinder::Exception);
}

TEST (AudioDataTest, FrameAccessBeforeChannelsInitialised) {
  KeyFinder::AudioData a;
  a.addToSampleCount(4);
  ASSERT_THROW(a.getSample(0, 0), KeyFinder::Exception);
}

TEST (AudioDataTest, FrameMutator) {
  KeyFinder::AudioData a;
  a.setChannels(4);
  a.addToFrameCount(5);
  ASSERT_EQ(5, a.getFrameCount());
  ASSERT_EQ(20, a.getSampleCount());

  a.setSample(6, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(6));
  ASSERT_FLOAT_EQ(10.0, a.getSample(1, 2));
  a.setSample(1, 2, 20.0);
  ASSERT_FLOAT_EQ(20.0, a.getSample(6));
  ASSERT_FLOAT_EQ(20.0, a.getSample(1, 2));
}

TEST (AudioDataTest, FrameMutatorBounds) {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.addToFrameCount(10);
  ASSERT_THROW(a.getSample(-1, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSample(10, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 0,-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample( 0, 2), KeyFinder::Exception);
}

TEST (AudioDataTest, AppendToNew) {
  KeyFinder::AudioData a;
  KeyFinder::AudioData b;

  ASSERT_NO_THROW(a.append(b));
  ASSERT_EQ(0, a.getChannels());
  ASSERT_EQ(0, a.getFrameRate());

  b.setChannels(1);
  b.setFrameRate(1);
  b.addToFrameCount(1);

  ASSERT_NO_THROW(a.append(b));
  ASSERT_EQ(1, a.getChannels());
  ASSERT_EQ(1, a.getFrameRate());
  ASSERT_EQ(1, a.getFrameCount());
}

TEST (AudioDataTest, AppendToInitialised) {
  KeyFinder::AudioData a;
  KeyFinder::AudioData b;

  a.setChannels(1);
  a.setFrameRate(1);
  ASSERT_THROW(a.append(b), KeyFinder::Exception);

  b.setChannels(2);
  b.setFrameRate(1);
  ASSERT_THROW(a.append(b), KeyFinder::Exception);

  b.setChannels(1);
  b.setFrameRate(2);
  ASSERT_THROW(a.append(b), KeyFinder::Exception);

  b.setChannels(1);
  b.setFrameRate(1);
  ASSERT_NO_THROW(a.append(b));

  a.addToFrameCount(1);
  b.addToFrameCount(1);
  a.setSample(0, 0, 10.0);
  b.setSample(0, 0, 20.0);

  a.append(b);
  ASSERT_EQ(2, a.getFrameCount());
  ASSERT_FLOAT_EQ(10.0, a.getSample(0, 0));
  ASSERT_FLOAT_EQ(20.0, a.getSample(1, 0));
}

TEST (AudioDataTest, DiscardFromFront) {
  KeyFinder::AudioData a;

  a.setChannels(1);
  a.setFrameRate(1);
  a.addToFrameCount(10);

  ASSERT_THROW(a.discardFramesFromFront(11), KeyFinder::Exception);
  ASSERT_NO_THROW(a.discardFramesFromFront(0));

  a.setSample(5, 0, 10.0);
  ASSERT_NO_THROW(a.discardFramesFromFront(5));
  ASSERT_EQ(5, a.getFrameCount());
  ASSERT_FLOAT_EQ(10.0, a.getSample(0, 0));
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
