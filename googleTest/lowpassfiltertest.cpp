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

#include "lowpassfiltertest.h"

unsigned int samples = 44100;
float magnitude = 32768.0;
// TODO: 5% tolerance is not ideal, and only works when
// target frequencies are very far from corner.
// Do some manual tests and compare to libSRC.
float tolerance = 0.05 * magnitude;

float highFrequency = 21000.0;
float lowFrequency = 800.0;
float cornerFrequency = 6500.0;

unsigned int filterOrder = 160;
unsigned int filterFFT = 2048;

TEST(LowPassFilterTest, KillsHigherFreqs){
  // make a high frequency sine wave, one second long
  KeyFinder::AudioData* a = new KeyFinder::AudioData();
  a->setChannels(1);
  a->setFrameRate(samples);
  a->addToSampleCount(samples);
  for(unsigned int i = 0; i < samples; i++){
    a->setSample(i, sine_wave(i, highFrequency, samples, magnitude));
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, samples, cornerFrequency, filterFFT);
  lpf->filter(a);
  delete lpf;

  // test for near silence
  for(unsigned int i = 0; i < samples; i++){
    ASSERT_GT(tolerance, a->getSample(i));
  }
}

TEST(LowPassFilterTest, MaintainsLowerFreqs){
  // make a low frequency sine wave, one second long
  KeyFinder::AudioData* a = new KeyFinder::AudioData();
  a->setChannels(1);
  a->setFrameRate(samples);
  a->addToSampleCount(samples);
  for(unsigned int i = 0; i < samples; i++){
    a->setSample(i, sine_wave(i, lowFrequency, samples, magnitude));
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, samples, cornerFrequency, filterFFT);
  lpf->filter(a);
  delete lpf;

  // test for near perfect reproduction
  for(unsigned int i = 0; i < samples; i++){
    float expected = sine_wave(i, lowFrequency, samples, magnitude);
    float min = expected - tolerance;
    float max = expected + tolerance;
    ASSERT_LT(min, a->getSample(i));
    ASSERT_GT(max, a->getSample(i));
  }
}

TEST(LowPassFilterTest, DoesBothAtOnce){
  // make two sine waves, one second long
  KeyFinder::AudioData* a = new KeyFinder::AudioData();
  a->setChannels(1);
  a->setFrameRate(samples);
  a->addToSampleCount(samples);
  for(unsigned int i = 0; i < samples; i++){
    float sample = 0;
    // high freq
    sample += sine_wave(i, highFrequency, samples, magnitude);
    // low freq
    sample += sine_wave(i, lowFrequency, samples, magnitude);
    a->setSample(i, sample);
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, samples, cornerFrequency, filterFFT);
  lpf->filter(a);
  delete lpf;

  // test for lower wave only
  for(unsigned int i = 0; i < samples; i++){
    float expected = sine_wave(i, lowFrequency, samples, magnitude);
    float min = expected - tolerance;
    float max = expected + tolerance;
    ASSERT_LT(min, a->getSample(i));
    ASSERT_GT(max, a->getSample(i));
  }
}
