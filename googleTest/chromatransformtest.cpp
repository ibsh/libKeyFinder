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

#include "chromatransformtest.h"

TEST (ChromaTransformTest, InsistsOnPositiveFrameRate) {
  KeyFinder::Parameters params;
  KeyFinder::ChromaTransform* ct = NULL;
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(0, params), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(4410, params));
}

TEST (ChromaTransformTest, InsistsOnNyquistHigherThanAnalysisFreqs) {
  KeyFinder::Parameters params;
  float high = params.getLastFrequency();
  KeyFinder::ChromaTransform* ct = NULL;
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(high * 2 - 1, params), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(high * 2 + 1, params));
}

TEST (ChromaTransformTest, InsistsOnSufficientBassResolution) {
  KeyFinder::Parameters params;
  float lowestDiff = params.getBandFrequency(1) - params.getBandFrequency(0);
  unsigned int frameRate = 4410;
  float minimumFftFrameSize = frameRate / lowestDiff;
  KeyFinder::ChromaTransform* ct = NULL;
  params.setFftFrameSize(minimumFftFrameSize - 1);
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(frameRate, params), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  params.setFftFrameSize(minimumFftFrameSize + 1);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(frameRate, params));
}

// Inheritance so we can get the (private) kernel out.
class MyChromaTransform : public KeyFinder::ChromaTransform {
public:
  MyChromaTransform(unsigned int f, const KeyFinder::Parameters p)
    : KeyFinder::ChromaTransform(f, p) { }
  std::vector<unsigned int> getChromaBandFftBinOffsets() { return chromaBandFftBinOffsets; }
  std::vector< std::vector<float> > getDirectSpectralKernel() { return directSpectralKernel; }
};

TEST (ChromaTransformTest, TestSpectralKernel) {
  KeyFinder::Parameters params;
  MyChromaTransform* myCt = NULL;
  myCt = new MyChromaTransform(4410, params);
  std::vector<unsigned int> cbfbo = myCt->getChromaBandFftBinOffsets();
  std::vector< std::vector<float> > dsk = myCt->getDirectSpectralKernel();
  unsigned int chromaBands = params.getOctaves() * params.getBandsPerOctave();

  // ensure correct element sizes
  ASSERT_EQ(chromaBands, cbfbo.size());
  ASSERT_EQ(chromaBands, dsk.size());

  // ensure offsets and sizes increase as frequency increases
  for (unsigned int i = 1; i < chromaBands; i++) {
    ASSERT_GT(cbfbo[i], cbfbo[i-1]);
    ASSERT_GE(dsk[i].size(), dsk[i-1].size());
  }

  // ensure that the relationship between frequency and bandwidth is constantish
  float q = params.getLastFrequency() / dsk[dsk.size() - 1].size();
  for (unsigned int i = 0; i < chromaBands; i++) {
    ASSERT_NEAR(q, params.getBandFrequency(i) / dsk[i].size(), q / 6);
  }

  // ensure that each kernel element is an up-and-down curve,
  // and that the peak is the vector's centre +/- 1,
  // and that the peak is at the expected frequency.
  for (unsigned int i = 0; i < chromaBands; i++) {
    int peak = -1;
    for (unsigned int j = 1; j < dsk[i].size(); j++) {
      if (peak < 0) {
        if(dsk[i][j] <= dsk[i][j-1]) peak = j-1;
      } else {
        ASSERT_LT(dsk[i][j], dsk[i][j-1]);
      }
    }
    ASSERT_NEAR(dsk[i].size() / 2, peak, 1);
    float peakFrequency = (cbfbo[i] + peak) * 4410.0 / params.getFftFrameSize();
    ASSERT_NEAR(params.getBandFrequency(i), peakFrequency, 0.2);
  }
}
