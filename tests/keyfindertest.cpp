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

TEST (KeyFinderTest, BasicUseCase) {
  unsigned int sampleRate = 44100;
  KeyFinder::AudioData inputAudio;
  inputAudio.setChannels(1);
  inputAudio.setFrameRate(sampleRate);
  inputAudio.addToSampleCount(sampleRate);
  for (unsigned int i = 0; i < sampleRate; i++) {
    float sample = 0.0;
    sample += sine_wave(i, 440.0000, sampleRate, 1);
    sample += sine_wave(i, 523.2511, sampleRate, 1);
    sample += sine_wave(i, 659.2551, sampleRate, 1);
    inputAudio.setSample(i, sample);
  }
  KeyFinder::KeyFinder kf;
  ASSERT_EQ(KeyFinder::A_MINOR, kf.keyOfAudio(inputAudio));
}

TEST (KeyFinderTest, ProgressiveUseCase) {

  /*
   * Build a second of audio, to be added ten times. The default settings will
   * lead to a downsample factor of 10, so there'll be 44100 samples of audio
   * after pre-processing. That'll be 7 hops, with 15428 samples left in the
   * buffer. Then finish that off with finalChromagramOfAudio, which should add
   * 4 more hops and leave 12288 zeroed samples in the buffer.
   */

  unsigned int sampleRate = 44100;
  KeyFinder::AudioData inputAudio;
  inputAudio.setFrameRate(sampleRate);
  inputAudio.setChannels(1);
  inputAudio.addToSampleCount(sampleRate);
  for (unsigned int i = 0; i < sampleRate; i++) {
    float sample = 0.0;
    sample += sine_wave(i, 440.0000, sampleRate, 1);
    sample += sine_wave(i, 523.2511, sampleRate, 1);
    sample += sine_wave(i, 659.2551, sampleRate, 1);
    inputAudio.setSample(i, sample);
  }

  /*
   * Add an annoying bit of silence at the beginning to mess with our perfect
   * integral relationship with the downsample factor, though it shouldn't alter
   * the numbers above.
   */

  KeyFinder::AudioData offset;
  offset.setFrameRate(sampleRate);
  offset.setChannels(1);
  offset.addToSampleCount(4);

  KeyFinder::KeyFinder k;
  KeyFinder::Workspace w;
  KeyFinder::FftAdapter* testFftPointer = NULL;

  k.progressiveChromagram(offset, w);
  ASSERT_EQ(4, w.remainderBuffer.getSampleCount());
  for (unsigned int i = 0; i < 10; i++) {
    k.progressiveChromagram(inputAudio, w);
    // ensure we're using the same FFT adapter throughout
    if (testFftPointer == NULL) testFftPointer = w.fftAdapter;
    ASSERT_EQ(testFftPointer, w.fftAdapter);
    ASSERT_EQ(4410, w.preprocessedBuffer.getFrameRate());
    ASSERT_EQ(1, w.preprocessedBuffer.getChannels());
    // check that the offset left some unprocessed audio in the remainder
    ASSERT_EQ(4, w.remainderBuffer.getSampleCount());
    // and that the remainder is equal to the last 4 samples which were excluded
    for (unsigned int j = 0; j < 4; j++) {
      ASSERT_FLOAT_EQ(
        inputAudio.getSample(inputAudio.getSampleCount() - 4 + j),
        w.remainderBuffer.getSample(j)
      );
    }
  }

  // progressive result without emptying preprocessedBuffer
  ASSERT_EQ(7, w.chromagram->getHops());
  ASSERT_EQ(15428, w.preprocessedBuffer.getSampleCount());

  // after emptying preprocessedBuffer
  k.finalChromagram(w);
  ASSERT_EQ(0, w.remainderBuffer.getSampleCount());
  ASSERT_EQ(11, w.chromagram->getHops());
  ASSERT_EQ(12288, w.preprocessedBuffer.getSampleCount());

  for (unsigned int i = 0; i < w.preprocessedBuffer.getSampleCount(); i++) {
    ASSERT_FLOAT_EQ(0.0, w.preprocessedBuffer.getSample(i));
  }

  ASSERT_EQ(KeyFinder::A_MINOR, k.keyOfChromagram(w));
}

TEST (KeyFinderTest, KeyOfChromagramReturnsSilence) {
  KeyFinder::Workspace w;
  w.chromagram = new KeyFinder::Chromagram(1);
  KeyFinder::KeyFinder kf;
  ASSERT_EQ(KeyFinder::SILENCE, kf.keyOfChromagram(w));
}

TEST (KeyFinderTest, KeyOfChromagramPassesThroughChromaData) {
  KeyFinder::Workspace w;
  w.chromagram = new KeyFinder::Chromagram(1);
  w.chromagram->setMagnitude(0, 24 + 0, 10000.0);
  w.chromagram->setMagnitude(0, 24 + 3, 10000.0);
  w.chromagram->setMagnitude(0, 24 + 7, 10000.0);
  KeyFinder::KeyFinder kf;

  ASSERT_EQ(KeyFinder::C_MINOR, kf.keyOfChromagram(w));
}

TEST (KeyFinderTest, KeyOfChromagramCollapsesTimeDimension) {
  KeyFinder::Workspace w;
  w.chromagram = new KeyFinder::Chromagram(5);
  w.chromagram->setMagnitude(1, 24 + 0, 1.0);
  w.chromagram->setMagnitude(2, 24 + 3, 1.0);
  w.chromagram->setMagnitude(3, 24 + 7, 1.0);
  KeyFinder::KeyFinder kf;
  ASSERT_EQ(KeyFinder::C_MINOR, kf.keyOfChromagram(w));
}
