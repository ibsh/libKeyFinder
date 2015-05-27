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

TEST_CASE ("AudioDataTest/ConstructorWorks") {
  KeyFinder::AudioData a;
  ASSERT_EQ(0, a.getChannels());
  ASSERT_EQ(0, a.getFrameRate());
  ASSERT_EQ(0, a.getSampleCount());
}

TEST_CASE ("AudioDataTest/Channels") {
  KeyFinder::AudioData a;
  a.setChannels(2);
  ASSERT_EQ(2, a.getChannels());
  ASSERT_THROW(a.setChannels(0), KeyFinder::Exception);
}

TEST_CASE ("AudioDataTest/FrameRate") {
  KeyFinder::AudioData a;
  a.setFrameRate(44100);
  ASSERT_EQ(44100, a.getFrameRate());
  ASSERT_THROW(a.setFrameRate(0), KeyFinder::Exception);
}

TEST_CASE ("AudioDataTest/SampleInitialisation") {
  KeyFinder::AudioData a;
  a.addToSampleCount(100);
  ASSERT_EQ(100, a.getSampleCount());
  // init values
  for (int i = 0; i < 100; i++) {
    ASSERT(0.0 == a.getSample(i));
  }
}

TEST_CASE ("AudioDataTest/SampleMutator") {
  KeyFinder::AudioData a;
  a.addToSampleCount(1);
  a.setSample(0, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(0));
}

TEST_CASE ("AudioDataTest/SampleMutatorBounds") {
  KeyFinder::AudioData a;
  a.addToSampleCount(5);
  ASSERT_THROW(a.getSample(-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSample(5), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(-1, 1.0), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(5, 1.0), KeyFinder::Exception);

  ASSERT_THROW(a.setSample(0, INFINITY), KeyFinder::Exception);
  ASSERT_THROW(a.setSample(0, NAN), KeyFinder::Exception);
}

TEST_CASE ("AudioDataTest/FrameAccessBeforeChannelsInitialised") {
  KeyFinder::AudioData a;
  a.addToSampleCount(4);
  ASSERT_THROW(a.getSampleByFrame(0, 0), KeyFinder::Exception);
}

TEST_CASE ("AudioDataTest/FrameMutator") {
  KeyFinder::AudioData a;
  a.setChannels(4);
  a.addToFrameCount(5);
  ASSERT_EQ(5, a.getFrameCount());
  ASSERT_EQ(20, a.getSampleCount());

  a.setSample(6, 10.0);
  ASSERT_FLOAT_EQ(10.0, a.getSample(6));
  ASSERT_FLOAT_EQ(10.0, a.getSampleByFrame(1, 2));
  a.setSampleByFrame(1, 2, 20.0);
  ASSERT_FLOAT_EQ(20.0, a.getSample(6));
  ASSERT_FLOAT_EQ(20.0, a.getSampleByFrame(1, 2));
}

TEST_CASE ("AudioDataTest/FrameMutatorBounds") {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.addToFrameCount(10);
  ASSERT_THROW(a.getSampleByFrame(-1, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSampleByFrame(10, 0), KeyFinder::Exception);
  ASSERT_THROW(a.getSampleByFrame( 0,-1), KeyFinder::Exception);
  ASSERT_THROW(a.getSampleByFrame( 0, 2), KeyFinder::Exception);
}

TEST_CASE ("AudioDataTest/AppendToNew") {
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

TEST_CASE ("AudioDataTest/PrependToNew") {
  KeyFinder::AudioData a;
  KeyFinder::AudioData b;

  ASSERT_NO_THROW(a.prepend(b));
  ASSERT_EQ(0, a.getChannels());
  ASSERT_EQ(0, a.getFrameRate());

  b.setChannels(1);
  b.setFrameRate(1);
  b.addToFrameCount(1);

  ASSERT_NO_THROW(a.prepend(b));
  ASSERT_EQ(1, a.getChannels());
  ASSERT_EQ(1, a.getFrameRate());
  ASSERT_EQ(1, a.getFrameCount());
}

TEST_CASE ("AudioDataTest/AppendToInitialised") {
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
  a.setSampleByFrame(0, 0, 10.0);
  b.setSampleByFrame(0, 0, 20.0);

  a.append(b);
  ASSERT_EQ(2, a.getFrameCount());
  ASSERT_FLOAT_EQ(10.0, a.getSampleByFrame(0, 0));
  ASSERT_FLOAT_EQ(20.0, a.getSampleByFrame(1, 0));
}

TEST_CASE ("AudioDataTest/PrependToInitialised") {
  KeyFinder::AudioData a;
  KeyFinder::AudioData b;

  a.setChannels(1);
  a.setFrameRate(1);
  ASSERT_THROW(a.prepend(b), KeyFinder::Exception);

  b.setChannels(2);
  b.setFrameRate(1);
  ASSERT_THROW(a.prepend(b), KeyFinder::Exception);

  b.setChannels(1);
  b.setFrameRate(2);
  ASSERT_THROW(a.prepend(b), KeyFinder::Exception);

  b.setChannels(1);
  b.setFrameRate(1);
  ASSERT_NO_THROW(a.prepend(b));

  a.addToFrameCount(1);
  b.addToFrameCount(1);
  a.setSampleByFrame(0, 0, 10.0);
  b.setSampleByFrame(0, 0, 20.0);

  a.prepend(b);
  ASSERT_EQ(2, a.getFrameCount());
  ASSERT_FLOAT_EQ(20.0, a.getSampleByFrame(0, 0));
  ASSERT_FLOAT_EQ(10.0, a.getSampleByFrame(1, 0));
}

TEST_CASE ("AudioDataTest/DiscardFromFront") {
  KeyFinder::AudioData a;

  a.setChannels(1);
  a.setFrameRate(1);

  ASSERT_THROW(a.discardFramesFromFront(1), KeyFinder::Exception);
  a.addToFrameCount(10);
  ASSERT_THROW(a.discardFramesFromFront(11), KeyFinder::Exception);
  ASSERT_NO_THROW(a.discardFramesFromFront(0));
  a.setSampleByFrame(5, 0, 10.0);
  ASSERT_NO_THROW(a.discardFramesFromFront(5));
  ASSERT_EQ(5, a.getFrameCount());
  ASSERT_FLOAT_EQ(10.0, a.getSampleByFrame(0, 0));
}

TEST_CASE ("AudioDataTest/SliceFromBack") {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(1);

  KeyFinder::AudioData* b = NULL;
  KeyFinder::AudioData* nullPtr = NULL;

  ASSERT_THROW(b = a.sliceSamplesFromBack(1), KeyFinder::Exception);
  ASSERT_EQ(nullPtr, b);

  a.addToFrameCount(10);
  ASSERT_THROW(b = a.sliceSamplesFromBack(11), KeyFinder::Exception);
  ASSERT_EQ(nullPtr, b);

  a.resetIterators();
  float v = 0;
  while (a.writeIteratorWithinUpperBound()) {
    a.setSampleAtWriteIterator(v);
    a.advanceWriteIterator();
    v += 1.0;
  }

  ASSERT_NO_THROW(b = a.sliceSamplesFromBack(5));
  ASSERT_NE(nullPtr, b);
  ASSERT_EQ(5, a.getSampleCount());
  ASSERT_EQ(5, b->getSampleCount());
  ASSERT_FLOAT_EQ(5.0, b->getSample(0));
  ASSERT_FLOAT_EQ(9.0, b->getSample(4));
  delete b;
}

TEST_CASE ("AudioDataTest/MakeMono") {
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

TEST_CASE ("AudioDataTest/DownsamplerInsistsOnMonophonicAudio") {
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.setFrameRate(100);
  a.addToSampleCount(10);

  ASSERT_THROW(a.downsample(5), KeyFinder::Exception);
  a.reduceToMono();
  ASSERT_NO_THROW(a.downsample(5));
}

TEST_CASE ("AudioDataTest/DownsamplerResamplesIntegralRelationship") {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(100);
  a.addToSampleCount(10);
  for (unsigned int i = 0; i < 5; i++)
    a.setSample(i, 100.0);
  for (unsigned int i = 5; i < 10; i++)
    a.setSample(i, 500.0);

  a.downsample(5);

  ASSERT_EQ(20, a.getFrameRate());
  ASSERT_EQ(2, a.getSampleCount());
  ASSERT_FLOAT_EQ(100.0, a.getSample(0));
  ASSERT_FLOAT_EQ(500.0, a.getSample(1));
}

TEST_CASE ("AudioDataTest/DownsamplerResamplesNonintegralRelationship") {
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

  a.downsample(5);

  ASSERT_EQ(3, a.getSampleCount());
  ASSERT_FLOAT_EQ(100.0, a.getSample(0));
  ASSERT_FLOAT_EQ(500.0, a.getSample(1));
  // this doesn't make total mathematical sense but I'm taking a shortcut for performance
  ASSERT_FLOAT_EQ(1000.0, a.getSample(2));
}

TEST_CASE ("AudioDataTest/DownsamplerResamplesSineWave") {
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

  a.downsample(factor);

  unsigned int newFrameRate = frameRate / factor;
  unsigned int newFrames = frames / factor;

  ASSERT_EQ(newFrameRate, a.getFrameRate());
  ASSERT_EQ(newFrames, a.getSampleCount());
  for (unsigned int i = 0; i < newFrames; i++) {
    ASSERT_NEAR(sine_wave(i, freq, newFrameRate, magnitude), a.getSample(i), magnitude * 0.05);
  }
}

TEST_CASE ("AudioDataTest/Iterators") {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(1);
  a.addToSampleCount(10);

  a.setSample(0, 10.0);
  a.setSample(1, 20.0);
  a.setSample(3, 50.0);

  a.resetIterators(); // this is required before each use

  ASSERT_FLOAT_EQ(10.0, a.getSampleAtReadIterator());
  a.setSampleAtWriteIterator(15.0);
  ASSERT_FLOAT_EQ(15.0, a.getSampleAtReadIterator());

  a.advanceReadIterator();
  a.advanceWriteIterator();
  ASSERT_FLOAT_EQ(20.0, a.getSampleAtReadIterator());
  a.setSampleAtWriteIterator(25.0);
  ASSERT_FLOAT_EQ(25.0, a.getSampleAtReadIterator());

  a.advanceReadIterator(2);
  a.advanceWriteIterator(2);
  ASSERT_FLOAT_EQ(50.0, a.getSampleAtReadIterator());
  a.setSampleAtWriteIterator(55.0);
  ASSERT_FLOAT_EQ(55.0, a.getSampleAtReadIterator());

  a.resetIterators();
  ASSERT_FLOAT_EQ(15.0, a.getSampleAtReadIterator());
  a.setSampleAtWriteIterator(150.0);
  ASSERT_FLOAT_EQ(150.0, a.getSampleAtReadIterator());

  ASSERT_TRUE(a.readIteratorWithinUpperBound());
  ASSERT_TRUE(a.writeIteratorWithinUpperBound());
  a.advanceReadIterator(10);
  a.advanceWriteIterator(10);
  ASSERT_FALSE(a.readIteratorWithinUpperBound());
  ASSERT_FALSE(a.writeIteratorWithinUpperBound());
}
