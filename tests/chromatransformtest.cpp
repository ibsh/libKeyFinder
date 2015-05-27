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

TEST (ChromaTransformTest, InsistsOnPositiveFrameRate) {
  KeyFinder::ChromaTransform* ct = NULL;
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(0), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(4410));
  ASSERT_NO_THROW(delete ct);
}

TEST (ChromaTransformTest, InsistsOnNyquistHigherThanAnalysisFreqs) {
  float high = KeyFinder::getLastFrequency();
  KeyFinder::ChromaTransform* ct = NULL;
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(high * 2 - 1), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(high * 2 + 1));
  delete ct;
}

TEST (ChromaTransformTest, InsistsOnSufficientBassResolution) {
  KeyFinder::ChromaTransform* ct = NULL;
  ASSERT_THROW(ct = new KeyFinder::ChromaTransform(31861), KeyFinder::Exception);
  ASSERT_EQ(NULL, ct);
  ASSERT_NO_THROW(ct = new KeyFinder::ChromaTransform(31860));
  delete ct;
}

// Inheritance so we can get the (private) kernel out.
class MyChromaTransform : public KeyFinder::ChromaTransform {
public:
  MyChromaTransform(unsigned int f) : KeyFinder::ChromaTransform(f) { }
  std::vector<unsigned int> getChromaBandFftBinOffsets() { return chromaBandFftBinOffsets; }
  std::vector< std::vector<double> > getDirectSpectralKernel() { return directSpectralKernel; }
};

/*TEST (ChromaTransformTest, TestSpectralKernel) {
  MyChromaTransform* myCt = NULL;
  myCt = new MyChromaTransform(4410);
  std::vector<unsigned int> cbfbo = myCt->getChromaBandFftBinOffsets();
  std::vector< std::vector<double> > dsk = myCt->getDirectSpectralKernel();
  delete myCt;

  // ensure correct element sizes
  ASSERT_EQ(BANDS, cbfbo.size());
  ASSERT_EQ(BANDS, dsk.size());

  // ensure offsets and sizes increase as frequency increases
  for (unsigned int i = 1; i < BANDS; i++) {
    ASSERT_GT(cbfbo[i], cbfbo[i-1]);
    ASSERT_GE(dsk[i].size(), dsk[i-1].size());
  }

  // ensure that the relationship between frequency and bandwidth is constantish
  float q = KeyFinder::getLastFrequency() / dsk[dsk.size() - 1].size();
  for (unsigned int i = 0; i < BANDS; i++) {
    ASSERT_NEAR(q, KeyFinder::getFrequencyOfBand(i) / dsk[i].size(), q / 6);
  }

  // ensure that each kernel element is an up-and-down curve,
  // and that the peak is the vector's centre +/- 1,
  // and that the peak is at the expected frequency.
  for (unsigned int i = 0; i < BANDS; i++) {
    int peak = -1;
    for (unsigned int j = 1; j < dsk[i].size(); j++) {
      if (peak < 0) {
        if(dsk[i][j] <= dsk[i][j-1]) peak = j-1;
      } else {
        ASSERT_LT(dsk[i][j], dsk[i][j-1]);
      }
    }
    ASSERT_NEAR((dsk[i].size() / 2), peak, 1);
    float peakFrequency = (cbfbo[i] + peak) * 4410.0 / FFTFRAMESIZE;
    ASSERT_NEAR(KeyFinder::getFrequencyOfBand(i), peakFrequency, 0.2);
  }
}*/
