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

#include "parameterstest.h"

TEST(ParametersTest, DefaultsAndAccessors){
  KeyFinder::Parameters p;

  // bools
  ASSERT_EQ(true, p.getOffsetToC());
  // enums
  ASSERT_EQ(KeyFinder::WINDOW_BLACKMAN,     p.getTemporalWindow());
  ASSERT_EQ(KeyFinder::SEGMENTATION_NONE,   p.getSegmentation());
  ASSERT_EQ(KeyFinder::SIMILARITY_COSINE,   p.getSimilarityMeasure());
  ASSERT_EQ(KeyFinder::TONE_PROFILE_SHAATH, p.getToneProfile());
  ASSERT_EQ(KeyFinder::TUNING_HARTE,        p.getTuningMethod());
  // unsigned ints
  ASSERT_EQ(16384, p.getFftFrameSize());
  ASSERT_EQ(4,     p.getHopsPerFrame());
  ASSERT_EQ(4096,  p.getHopSize());
  ASSERT_EQ(6,     p.getOctaves());
  ASSERT_EQ(1,     p.getBandsPerSemitone());
  ASSERT_EQ(12,    p.getBandsPerOctave());
  ASSERT_EQ(4,     p.getSegPeakPickingNeighbours());
  ASSERT_EQ(3,     p.getArbitrarySegments());
  ASSERT_EQ(35,    p.getSegGaussianSize());
  // floats
  ASSERT_FLOAT_EQ(8.0,  p.getSegGaussianSigma());
  ASSERT_FLOAT_EQ(27.5, p.getStartingFreqA());
  ASSERT_FLOAT_EQ(0.8,  p.getDirectSkStretch());
  ASSERT_FLOAT_EQ(0.2,  p.getDetunedBandWeight());
  // custom tone profile
  std::vector<float> ctp = p.getCustomToneProfile();
  // major
  ASSERT_FLOAT_EQ(1.0, ctp[0]);
  ASSERT_FLOAT_EQ(0.0, ctp[1]);
  ASSERT_FLOAT_EQ(1.0, ctp[2]);
  ASSERT_FLOAT_EQ(0.0, ctp[3]);
  ASSERT_FLOAT_EQ(1.0, ctp[4]);
  ASSERT_FLOAT_EQ(1.0, ctp[5]);
  ASSERT_FLOAT_EQ(0.0, ctp[6]);
  ASSERT_FLOAT_EQ(1.0, ctp[7]);
  ASSERT_FLOAT_EQ(0.0, ctp[8]);
  ASSERT_FLOAT_EQ(1.0, ctp[9]);
  ASSERT_FLOAT_EQ(0.0, ctp[10]);
  ASSERT_FLOAT_EQ(1.0, ctp[11]);
  // minor
  ASSERT_FLOAT_EQ(1.0, ctp[12]);
  ASSERT_FLOAT_EQ(0.0, ctp[13]);
  ASSERT_FLOAT_EQ(1.0, ctp[14]);
  ASSERT_FLOAT_EQ(1.0, ctp[15]);
  ASSERT_FLOAT_EQ(0.0, ctp[16]);
  ASSERT_FLOAT_EQ(1.0, ctp[17]);
  ASSERT_FLOAT_EQ(0.0, ctp[18]);
  ASSERT_FLOAT_EQ(1.0, ctp[19]);
  ASSERT_FLOAT_EQ(1.0, ctp[20]);
  ASSERT_FLOAT_EQ(0.0, ctp[21]);
  ASSERT_FLOAT_EQ(1.0, ctp[22]);
  ASSERT_FLOAT_EQ(0.0, ctp[23]);
  // Some band frequencies
  ASSERT_NEAR(32.7,    p.getBandFrequency(0), 0.01);
  ASSERT_NEAR(55.0,    p.getBandFrequency(9), 0.01);
  ASSERT_NEAR(1975.53, p.getLastFrequency(),  0.01);
}

TEST(ParametersTest, FreqsWithoutOffset){
  KeyFinder::Parameters p;
  p.setOffsetToC(false);
  ASSERT_NEAR(27.5,    p.getBandFrequency(0),  0.01);
  ASSERT_NEAR(55.0,    p.getBandFrequency(12), 0.01);
  ASSERT_NEAR(110.0,   p.getBandFrequency(24), 0.01);
  ASSERT_NEAR(1661.22, p.getLastFrequency(),   0.01);
}

TEST(ParametersTest, FreqBounds){
  KeyFinder::Parameters p;
  ASSERT_THROW(p.getBandFrequency(-1), KeyFinder::Exception);
  ASSERT_NO_THROW(p.getBandFrequency(71));
  ASSERT_THROW(p.getBandFrequency(72), KeyFinder::Exception);
  p.setOctaves(1);
  ASSERT_NO_THROW(p.getBandFrequency(11));
  ASSERT_THROW(p.getBandFrequency(12), KeyFinder::Exception);
}

TEST(ParametersTest, StartingFrequencyMutator){
  KeyFinder::Parameters p;
  // Check that float comparisons are safe
  ASSERT_NO_THROW(p.setStartingFrequencyA(27.5));
  ASSERT_NO_THROW(p.setStartingFrequencyA(55.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(110.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(220.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(440.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(880.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(1760.0));
  ASSERT_NO_THROW(p.setStartingFrequencyA(3520.0));
  ASSERT_THROW(p.setStartingFrequencyA(27.4), KeyFinder::Exception);
}

TEST(ParametersTest, CustomToneProfileMutator){
  KeyFinder::Parameters p;
  p.setCustomToneProfile(std::vector<float>(24, 0.0));
  ASSERT_THROW(p.setCustomToneProfile(std::vector<float>(23, 0.0)), KeyFinder::Exception);
  ASSERT_THROW(p.setCustomToneProfile(std::vector<float>(25, 0.0)), KeyFinder::Exception);
  ASSERT_THROW(p.setCustomToneProfile(std::vector<float>(24, -0.1)), KeyFinder::Exception);
}

TEST(ParametersTest, OtherMutatorsWithValidation){
  KeyFinder::Parameters p;
  p.setFftFrameSize(1);
  ASSERT_THROW(p.setFftFrameSize(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setHopsPerFrame(1));
  ASSERT_THROW(p.setHopsPerFrame(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setOctaves(1));
  ASSERT_THROW(p.setOctaves(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setBandsPerSemitone(1));
  ASSERT_THROW(p.setBandsPerSemitone(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setArbitrarySegments(1));
  ASSERT_THROW(p.setArbitrarySegments(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setSegGaussianSize(1));
  ASSERT_THROW(p.setSegGaussianSize(0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setSegGaussianSigma(0.1));
  ASSERT_THROW(p.setSegGaussianSigma(0.0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setDirectSkStretch(0.1));
  ASSERT_THROW(p.setDirectSkStretch(0.0), KeyFinder::Exception);
  ASSERT_NO_THROW(p.setDetunedBandWeight(0.0));
  ASSERT_THROW(p.setDetunedBandWeight(-0.1), KeyFinder::Exception);
}

TEST(ParametersTest, FrequencyBandsForTuning){
  KeyFinder::Parameters p;
  p.setOctaves(1);
  p.setOffsetToC(false);
  p.setBandsPerSemitone(3);
  ASSERT_NEAR(26.98, p.getBandFrequency(0),  0.01);
  ASSERT_NEAR(27.5,  p.getBandFrequency(1),  0.01);
  ASSERT_NEAR(28.03, p.getBandFrequency(2),  0.01);
  ASSERT_NEAR(50.92, p.getBandFrequency(33), 0.01);
  ASSERT_NEAR(51.91,  p.getBandFrequency(34), 0.01);
  ASSERT_NEAR(52.92, p.getBandFrequency(35), 0.01);
}
