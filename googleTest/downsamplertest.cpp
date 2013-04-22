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

#include "downsamplertest.h"

TEST (DownsamplerTest, ResamplesOneChannel) {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(100);
  a.addToSampleCount(10);
  for (unsigned int i = 0; i < 5; i++)
    a.setSample(i, 100.0);
  for (unsigned int i = 5; i < 10; i++)
    a.setSample(i, 500.0);

  KeyFinder::Downsampler ds;
  ds.downsample(a, 5);

  ASSERT_EQ(20, a.getFrameRate());
  ASSERT_EQ(2, a.getSampleCount());
  ASSERT_FLOAT_EQ(100.0, a.getSample(0));
  ASSERT_FLOAT_EQ(500.0, a.getSample(1));
}

TEST (DownsamplerTest, ResamplesNonintegralRelationship) {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(100);
  a.addToSampleCount(12);
  for (unsigned int i = 0; i < 5; i++)
    a.setSample(i, 100.0);
  for (unsigned int i = 5; i < 10; i++)
    a.setSample(i, 500.0);
  for (unsigned int i = 10; i < 12; i++)
    a.setSample(i, 1000.0);

  KeyFinder::Downsampler ds;
  ds.downsample(a, 5);

  ASSERT_EQ(3, a.getSampleCount());
  ASSERT_FLOAT_EQ(100.0, a.getSample(0));
  ASSERT_FLOAT_EQ(500.0, a.getSample(1));
  ASSERT_FLOAT_EQ(400.0, a.getSample(2));
}

TEST (DownsamplerTest, ResamplesTwoChannels) {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.setFrameRate(100);
  a.addToFrameCount(6);
  for (unsigned int i = 0; i < a.getFrameCount(); i++) {
    a.setSample(i, 0, 100.0);
    a.setSample(i, 1, 200.0);
  }

  KeyFinder::Downsampler ds;
  ds.downsample(a, 5);

  ASSERT_EQ(2, a.getFrameCount());
  ASSERT_FLOAT_EQ(100.0, a.getSample(0));
  ASSERT_FLOAT_EQ(200.0, a.getSample(1));
  ASSERT_FLOAT_EQ(20.0, a.getSample(2));
  ASSERT_FLOAT_EQ(40.0, a.getSample(3));
}


TEST (DownsamplerTest, ResamplesMoreChannels) {
  KeyFinder::AudioData a;
  a.setChannels(5);
  a.setFrameRate(100);
  a.addToFrameCount(6);
  for (unsigned int i = 0; i < a.getFrameCount(); i++) {
    a.setSample(i, 0, 100.0);
    a.setSample(i, 1, 200.0);
    a.setSample(i, 2, 300.0);
    a.setSample(i, 3, 400.0);
    a.setSample(i, 4, 500.0);
  }

  KeyFinder::Downsampler ds;
  ds.downsample(a, 5);

  ASSERT_EQ(2, a.getFrameCount());

  ASSERT_FLOAT_EQ(100.0, a.getSample(0, 0));
  ASSERT_FLOAT_EQ(200.0, a.getSample(0, 1));
  ASSERT_FLOAT_EQ(300.0, a.getSample(0, 2));
  ASSERT_FLOAT_EQ(400.0, a.getSample(0, 3));
  ASSERT_FLOAT_EQ(500.0, a.getSample(0, 4));
  ASSERT_FLOAT_EQ( 20.0, a.getSample(1, 0));
  ASSERT_FLOAT_EQ( 40.0, a.getSample(1, 1));
  ASSERT_FLOAT_EQ( 60.0, a.getSample(1, 2));
  ASSERT_FLOAT_EQ( 80.0, a.getSample(1, 3));
  ASSERT_FLOAT_EQ(100.0, a.getSample(1, 4));
}
