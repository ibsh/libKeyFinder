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

#include "windowfunctions.h"

namespace KeyFinder {

  float WindowFunction::window(temporal_window_t w, int n, int N) const {
    if (w == WINDOW_HANN) {
      return 0.5 * (1.0 - cos((2 * PI * n)/(N-1)));
    } else if (w == WINDOW_HAMMING) {
      return 0.54 - (0.46 * cos((2 * PI * n)/(N-1)));
    } else { // Blackman
      return 0.42 - (0.5 * cos((2 * PI * n)/(N-1))) + (0.08 * cos((4 * PI * n)/(N-1)));
    }
  }

  float WindowFunction::gaussianWindow(int n, int N, float sigma) const {
    return exp(-1 * (pow(double(n) - double(N / 2), 2) / (2 * sigma * sigma)));;
  }

  std::vector<float> WindowFunction::convolve(
    const std::vector<float>& input, const std::vector<float>& window
  ) const {
    unsigned int inputSize = input.size();
    unsigned int padding = window.size() / 2;
    std::vector<float> convolved(inputSize, 0.0);
    // TODO: this implements zero padding for boundary effects,
    // write something mean-based later.
    for (unsigned int sample = 0; sample < inputSize; sample++) {
      float convolution = 0.0;
      for (unsigned int k = 0; k < window.size(); k++) {
        int frm = (signed)sample - (signed)padding + (signed)k;
        if (frm >= 0 && frm < (signed)inputSize) // don't run off either end
          convolution += input[frm] * window[k] / window.size();
      }
      convolved[sample] = convolution;
    }
    return convolved;
  }

}
