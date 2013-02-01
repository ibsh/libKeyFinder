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

#include "fftadaptertest.h"

TEST(FftAdapterTest, WorksForCleanOscillations){

  unsigned int frameSize = 2048;
  KeyFinder::FftAdapter f(frameSize);

  for(unsigned int i = 0; i < frameSize; i++){
    float sample = 0.0;
    sample += sine_wave(i,  2, frameSize, 10000);
    sample += sine_wave(i,  4, frameSize,  8000);
    sample += sine_wave(i,  5, frameSize,  6000);
    sample += sine_wave(i,  7, frameSize,  4000);
    sample += sine_wave(i, 13, frameSize,  2000);
    sample += sine_wave(i, 20, frameSize,   500);
    f.setInput(i, sample);
  }

  f.execute();

  for(unsigned int i = 0; i < frameSize / 2; i++){
    float out = f.getOutputMagnitude(i);
    if (i == 2){
      ASSERT_EQ(10000 / 2 * frameSize, out);
    } else if (i == 4){
      ASSERT_EQ(8000 / 2 * frameSize, out);
    } else if (i == 5){
      ASSERT_EQ(6000 / 2 * frameSize, out);
    } else if (i == 7){
      ASSERT_EQ(4000 / 2 * frameSize, out);
    } else if (i == 13){
      ASSERT_EQ(2000 / 2 * frameSize, out);
    } else if (i == 20){
      ASSERT_EQ(500 / 2 * frameSize, out);
    } else {
      ASSERT_GT(1, out);
    }
  }
}
