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

#include "keyfindertest.h"

TEST (KeyFinderTest, ChromagramOfAudioDetectsAMinorTriad) {
    unsigned int sampleRate = 44100;
    KeyFinder::AudioData a;
    a.setChannels(1);
    a.setFrameRate(sampleRate);
    a.addToSampleCount(sampleRate);
    for (unsigned int i = 0; i < sampleRate; i++) {
      float sample = 0.0;
      sample += sine_wave(i, 440.0000, sampleRate, 1);
      sample += sine_wave(i, 523.2511, sampleRate, 1);
      sample += sine_wave(i, 659.2551, sampleRate, 1);
      a.setSample(i, sample);
    }

    KeyFinder::KeyFinder kf;
    KeyFinder::Parameters p;
    p.setFftFrameSize(sampleRate);
    p.setOffsetToC(false);
    KeyFinder::Chromagram ch(kf.chromagramOfAudio(a, p));
    ASSERT_EQ(1, ch.getHops());
    ASSERT_EQ(72, ch.getBands());
    for (unsigned int b = 0; b < ch.getBands(); b++){
      if (b == 48 || b == 51 || b == 55)
        ASSERT_LT(175.0, ch.getMagnitude(0, b));
      else
        ASSERT_GT(16.0, ch.getMagnitude(0, b));
    }
}

TEST (KeyFinderTest, KeyOfChromagramReturnsSilence) {
  KeyFinder::Chromagram ch(1,1,1);
  KeyFinder::KeyFinder kf;
  KeyFinder::KeyDetectionResult kdr = kf.keyOfChromagram(ch);
  ASSERT_EQ(KeyFinder::SILENCE, kdr.globalKeyEstimate);
  ASSERT_EQ(1, kdr.segments.size());
  ASSERT_EQ(12, kdr.segments[0].chromaVector.size());
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[0]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[1]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[2]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[3]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[4]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[5]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[6]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[7]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[8]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[9]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[10]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[11]);
}

TEST (KeyFinderTest, KeyOfChromagramPassesThroughChromaData) {
  KeyFinder::Chromagram ch(1,1,1);
  ch.setMagnitude(0, 0, 1.0);
  ch.setMagnitude(0, 3, 1.0);
  ch.setMagnitude(0, 7, 1.0);
  KeyFinder::KeyFinder kf;
  KeyFinder::KeyDetectionResult kdr = kf.keyOfChromagram(ch);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[0]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[1]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[2]);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[3]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[4]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[5]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[6]);
  ASSERT_FLOAT_EQ(1.0, kdr.segments[0].chromaVector[7]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[8]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[9]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[10]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[11]);
  ASSERT_EQ(KeyFinder::C_MINOR, kdr.globalKeyEstimate);
}

TEST (KeyFinderTest, KeyOfChromagramCollapsesTimeDimension) {
  KeyFinder::Chromagram ch(5,1,1);
  for (int i = 0; i < 5; i++) {
    ch.setMagnitude(i, 0, 1.0);
    ch.setMagnitude(i, 3, 1.0);
    ch.setMagnitude(i, 7, 1.0);
  }
  KeyFinder::KeyFinder kf;
  KeyFinder::KeyDetectionResult kdr = kf.keyOfChromagram(ch);
  ASSERT_FLOAT_EQ(5.0, kdr.segments[0].chromaVector[0]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[1]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[2]);
  ASSERT_FLOAT_EQ(5.0, kdr.segments[0].chromaVector[3]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[4]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[5]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[6]);
  ASSERT_FLOAT_EQ(5.0, kdr.segments[0].chromaVector[7]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[8]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[9]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[10]);
  ASSERT_FLOAT_EQ(0.0, kdr.segments[0].chromaVector[11]);
  ASSERT_EQ(KeyFinder::C_MINOR, kdr.globalKeyEstimate);
}

TEST (KeyFinderTest, KeyOfAudioDetectsAMinorTriad) {
    unsigned int sampleRate = 44100;
    KeyFinder::AudioData a;
    a.setChannels(1);
    a.setFrameRate(sampleRate);
    a.addToSampleCount(sampleRate);
    for (unsigned int i = 0; i < sampleRate; i++) {
      float sample = 0.0;
      sample += sine_wave(i, 440.0000, sampleRate, 1);
      sample += sine_wave(i, 523.2511, sampleRate, 1);
      sample += sine_wave(i, 659.2551, sampleRate, 1);
      a.setSample(i, sample);
    }

    KeyFinder::KeyFinder kf;
    KeyFinder::Parameters p;
    p.setFftFrameSize(sampleRate);
    KeyFinder::KeyDetectionResult kdr(kf.keyOfAudio(a, p));
    ASSERT_EQ(KeyFinder::A_MINOR, kdr.globalKeyEstimate);
}

TEST (KeyFinderTest, FlatBeatRegressionTest) {
  KeyFinder::KeyFinder kf;
  KeyFinder::Parameters p;
  KeyFinder::KeyDetectionResult kdr;
  KeyFinder::Chromagram ch(1,1,1);

  // Flat Beat by Mr Oizo gets a different result from each of the tone
  // profiles; this chroma vector represents it.
  ch.setMagnitude(0,  0, 2236193024);
  ch.setMagnitude(0,  1, 1869016576);
  ch.setMagnitude(0,  2, 2052115584);
  ch.setMagnitude(0,  3, 1794053632);
  ch.setMagnitude(0,  4, 1920909568);
  ch.setMagnitude(0,  5, 1918255616);
  ch.setMagnitude(0,  6, 1902896640);
  ch.setMagnitude(0,  7, 2394525184);
  ch.setMagnitude(0,  8, 2541725952);
  ch.setMagnitude(0,  9, 3349090304);
  ch.setMagnitude(0, 10, 3699921408);
  ch.setMagnitude(0, 11, 3248228096);

  p.setToneProfile(KeyFinder::TONE_PROFILE_KRUMHANSL);
  kdr = kf.keyOfChromagram(ch, p);
  ASSERT_EQ(KeyFinder::G_MINOR, kdr.globalKeyEstimate);

  p.setToneProfile(KeyFinder::TONE_PROFILE_TEMPERLEY);
  kdr = kf.keyOfChromagram(ch, p);
  ASSERT_EQ(KeyFinder::B_FLAT_MAJOR, kdr.globalKeyEstimate);

  p.setToneProfile(KeyFinder::TONE_PROFILE_GOMEZ);
  kdr = kf.keyOfChromagram(ch, p);
  ASSERT_EQ(KeyFinder::E_FLAT_MINOR, kdr.globalKeyEstimate);

  p.setToneProfile(KeyFinder::TONE_PROFILE_SHAATH);
  kdr = kf.keyOfChromagram(ch, p);
  ASSERT_EQ(KeyFinder::B_MINOR, kdr.globalKeyEstimate);
}
