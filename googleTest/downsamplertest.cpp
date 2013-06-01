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

TEST (DownsamplerTest, InsistsOnMonophonicAudio) {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.setFrameRate(100);
  a.addToSampleCount(10);

  KeyFinder::Downsampler ds;

  ASSERT_THROW(ds.downsample(a, 5), KeyFinder::Exception);
  a.reduceToMono();
  ASSERT_NO_THROW(ds.downsample(a, 5));
}

TEST (DownsamplerTest, ResamplesIntegralRelationship) {
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
  // this doesn't make total mathematical sense but I'm taking a shortcut for performance
  ASSERT_FLOAT_EQ(1000.0, a.getSample(2));
}

TEST (DownsamplerTest, ResamplesSineWave) {
  unsigned int frameRate = 10000;
  unsigned int frames = frameRate * 4;
  float freq = 20;
  float magnitude = 32768.0;
  unsigned int factor = 5;

  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frames);
  for (unsigned int i = 0; i < frames; i++)
    a.setSample(i, sine_wave(i, freq, frameRate, magnitude));

  KeyFinder::Downsampler ds;
  ds.downsample(a, factor);

  unsigned int newFrameRate = frameRate / factor;
  unsigned int newFrames = frames / factor;

  ASSERT_EQ(newFrameRate, a.getFrameRate());
  ASSERT_EQ(newFrames, a.getSampleCount());
  for (unsigned int i = 0; i < newFrames; i++)
    ASSERT_NEAR(sine_wave(i, freq, newFrameRate, magnitude), a.getSample(i), magnitude * 0.05);
}
