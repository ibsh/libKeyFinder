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

unsigned int frameRate = 44100;
float magnitude = 32768.0;
// TODO: 5% tolerance is not ideal, and only works when
// target frequencies are far from corner.
// Do some manual tests and compare to libSRC.
float tolerance = 0.05 * magnitude;

float highFrequency = 21000.0;
float lowFrequency = 800.0;
float cornerFrequency = 6500.0;

unsigned int filterOrder = 160;
unsigned int filterFFT = 2048;

TEST (LowPassFilterTest, InsistsOnEvenOrder) {
  KeyFinder::LowPassFilter* lpf = NULL;
  ASSERT_THROW(lpf = new KeyFinder::LowPassFilter(filterOrder + 1, frameRate, cornerFrequency, filterFFT), KeyFinder::Exception);
  ASSERT_EQ(NULL, lpf);
}

TEST (LowPassFilterTest, InsistsOnOrderNotGreaterThanOneQuarterFftFrameSize) {
  KeyFinder::LowPassFilter* lpf = NULL;
  ASSERT_THROW(lpf = new KeyFinder::LowPassFilter(514, frameRate, cornerFrequency, 2048), KeyFinder::Exception);
  ASSERT_EQ(NULL, lpf);
  ASSERT_NO_THROW(lpf = new KeyFinder::LowPassFilter(512, frameRate, cornerFrequency, 2048));
}

TEST (LowPassFilterTest, InitialisesNullRingBuffer) {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frameRate);

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  KeyFinder::Binode<float>* p = NULL;
  lpf->filter(a, w);
  ASSERT_NE(p, w.getLpfBuffer());
  unsigned int count = 1;
  p = w.getLpfBuffer()->r;
  for (; p != w.getLpfBuffer(); count++) {
    p = p->r;
  }
  ASSERT_EQ(filterOrder + 1, count);
  delete lpf;
}

TEST (LowPassFilterTest, DoesntAlterAudioMetadata) {
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frameRate);

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  ASSERT_EQ(1, a.getChannels());
  ASSERT_EQ(frameRate, a.getFrameRate());
  ASSERT_EQ(frameRate, a.getSampleCount());
}

TEST (LowPassFilterTest, KillsHigherFreqs) {
  // make a high frequency sine wave, one second long
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frameRate);
  for (unsigned int i = 0; i < frameRate; i++) {
    a.setSample(i, sine_wave(i, highFrequency, frameRate, magnitude));
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  // test for near silence
  for (unsigned int i = 0; i < frameRate; i++) {
    ASSERT_NEAR(0.0, a.getSample(i), tolerance);
  }
}

TEST (LowPassFilterTest, MaintainsLowerFreqs) {
  // make a low frequency sine wave, one second long
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frameRate);
  for (unsigned int i = 0; i < frameRate; i++) {
    a.setSample(i, sine_wave(i, lowFrequency, frameRate, magnitude));
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  // test for near perfect reproduction
  for (unsigned int i = 0; i < frameRate; i++) {
    float expected = sine_wave(i, lowFrequency, frameRate, magnitude);
    ASSERT_NEAR(expected, a.getSample(i), tolerance);
  }
}

TEST (LowPassFilterTest, DoesBothAtOnce) {
  // make two sine waves, one second long
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frameRate);
  for (unsigned int i = 0; i < frameRate; i++) {
    float sample = 0.0;
    sample += sine_wave(i, highFrequency, frameRate, magnitude); // high freq
    sample += sine_wave(i, lowFrequency, frameRate, magnitude); // low freq
    a.setSample(i, sample);
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  // test for lower wave only
  for (unsigned int i = 0; i < frameRate; i++) {
    float expected = sine_wave(i, lowFrequency, frameRate, magnitude);
    ASSERT_NEAR(expected, a.getSample(i), tolerance);
  }
}

TEST (LowPassFilterTest, WorksOnRepetitiveWaves) {
  // make two sine waves, but this time, several seconds long
  unsigned int frames = frameRate * 5;
  KeyFinder::AudioData a;
  a.setChannels(1);
  a.setFrameRate(frameRate);
  a.addToSampleCount(frames);
  for (unsigned int i = 0; i < frames; i++) {
    float sample = 0.0;
    sample += sine_wave(i, highFrequency, frameRate, magnitude); // high freq
    sample += sine_wave(i, lowFrequency, frameRate, magnitude); // low freq
    a.setSample(i, 0, sample);
    // ensure repetition of sine waves is perfect...
    if (i >= frameRate) {
      ASSERT_NEAR(a.getSample(i), a.getSample(i - frameRate), tolerance);
    }
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  // test for lower wave only
  for (unsigned int i = 0; i < frames; i++) {
    float expected = sine_wave(i, lowFrequency, frameRate, magnitude);
    ASSERT_NEAR(expected, a.getSample(i), tolerance);
  }
}

TEST (LowPassFilterTest, WorksOnMultipleChannels) {
  // make two sine waves, one second long
  unsigned int channels = 5;
  KeyFinder::AudioData a;
  a.setChannels(channels);
  a.setFrameRate(frameRate);
  a.addToFrameCount(frameRate);
  for (unsigned int i = 0; i < frameRate; i++) {
    float sample = 0.0;
    sample += sine_wave(i, highFrequency, frameRate, magnitude); // high freq
    sample += sine_wave(i, lowFrequency, frameRate, magnitude); // low freq
    for (unsigned int j = 0; j < channels; j++) {
      a.setSample(i, j, sample);
    }
  }

  KeyFinder::LowPassFilter* lpf = new KeyFinder::LowPassFilter(filterOrder, frameRate, cornerFrequency, filterFFT);
  KeyFinder::Workspace w;
  lpf->filter(a, w);
  delete lpf;

  // test for lower wave only
  for (unsigned int i = 0; i < frameRate; i++) {
    float expected = sine_wave(i, lowFrequency, frameRate, magnitude);
    float test;
    for (unsigned int j = 0; j < channels; j++) {
      ASSERT_NEAR(expected, a.getSample(i, j), tolerance);
      if (j == 0)
        test = a.getSample(i, j);
      ASSERT_FLOAT_EQ(test, a.getSample(i, j));
    }
  }
}

// Inheritance so we can check the coefficients. Probably best to refactor instead.
class DefaultLowPassFilterWithPublicCoefficients : public KeyFinder::LowPassFilter {
public:
  DefaultLowPassFilterWithPublicCoefficients() : KeyFinder::LowPassFilter(160, 44100, 2000.0, 2048) {}
  std::vector<float> getCoefficients() const { return coefficients; }
};

TEST (LowPassFilterTest, DefaultFilterMatchesFisherCoefficients) {
  DefaultLowPassFilterWithPublicCoefficients* lpf = new DefaultLowPassFilterWithPublicCoefficients();
  std::vector<float> myCoeffs = lpf->getCoefficients();
  delete lpf;

  float fisherCoeffsFirstHalf[] = {
    -0.0022979864, -0.0014851155, -0.0005276345, +0.0005287637,
    +0.0016288105, +0.0027066298, +0.0036859262, +0.0044820600,
    +0.0050064517, +0.0051734225, +0.0049091760, +0.0041622026,
    +0.0029140060, +0.0011887658, -0.0009395862, -0.0033443515,
    -0.0058483343, -0.0082321768, -0.0102489292, -0.0116443067,
    -0.0121813339, -0.0116673677, -0.0099809222, -0.0070953669,
    -0.0030964983, +0.0018087642, +0.0072947272, +0.0129315999,
    +0.0182126619, +0.0225928091, +0.0255360681, +0.0265684688,
    +0.0253317039, +0.0216323992, +0.0154816648, +0.0071199603,
    -0.0029768131, -0.0141127078, -0.0254095608, -0.0358661777,
    -0.0444356705, -0.0501157252, -0.0520448654, -0.0495965416,
    -0.0424622921, -0.0307153754, -0.0148472270, +0.0042291942,
    +0.0252127139, +0.0464845605, +0.0662137647, +0.0824916099,
    +0.0934864451, +0.0976077458, +0.0936666466, +0.0810194757,
    +0.0596811993, +0.0303971839, -0.0053357703, -0.0453047237,
    -0.0866737087, -0.1261316811, -0.1600878564, -0.1849028543,
    -0.1971406561, -0.1938239736, -0.1726744703, -0.1323195052,
    -0.0724487288, +0.0060931437, +0.1012868940, +0.2099971950,
    +0.3281078087, +0.4507269541, +0.5724509503, +0.6876697384,
    +0.7908945043, +0.8770856432, +0.9419588972, +0.9822487143,
    +0.9959106445
  };

  for (unsigned int i = 0; i < 81; i++) {
    ASSERT_FLOAT_EQ(fisherCoeffsFirstHalf[i], myCoeffs[i]);
    ASSERT_FLOAT_EQ(myCoeffs[i], myCoeffs[160 - i]);
  }
}
