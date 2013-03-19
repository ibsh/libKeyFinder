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

#include "segmentation.h"

namespace KeyFinder {

  std::vector<unsigned int> Segmentation::getSegmentationBoundaries(
    const Chromagram& ch,
    const Parameters& params
  ) const {
    std::vector<unsigned int> segmentBoundaries(1, 0); // start vector with a 0 to enable first classification
    if (params.getSegmentation() == SEGMENTATION_ARBITRARY) {
      unsigned int segments = params.getArbitrarySegments();
      float interval = ch.getHops() / segments;
      for (unsigned int i = 1; i < segments; i++) {
        segmentBoundaries.push_back((unsigned int)(interval * i + 0.5));
      }
      return segmentBoundaries;
    } else if (params.getSegmentation() == SEGMENTATION_COSINE) {
      std::vector<float> rateOfChange = cosineRateOfChange(ch, params.getSegGaussianSize(), params.getSegGaussianSigma());
      unsigned int neighbours = params.getSegPeakPickingNeighbours();
      // for all hops except those in the neighbourhood of the extremities
      for (unsigned int hop = neighbours; hop < rateOfChange.size() - neighbours; hop++) {
        bool peak = true;
        // check that ROC is bigger than the neighbours in each direction
        for (int i = -neighbours; i <= (signed)neighbours; i++) {
          if (i == 0) continue;
          if (rateOfChange[hop] <= rateOfChange[hop+i]) peak = false;
        }
        if (peak) segmentBoundaries.push_back(hop);
      }
    }
    return segmentBoundaries;
  }

  std::vector<float> Segmentation::cosineRateOfChange(
    const Chromagram& ch,
    unsigned int gaussianSize,
    float gaussianSigma
  ) const {
    unsigned int hops = ch.getHops();
    unsigned int bands = ch.getBands();
    // initialise to 1.0 (implies vectors are exactly the same)
    std::vector<float> cosineChanges(hops, 1.0);
    // determine cosine similarity
    for (unsigned int hop = 0; hop < hops; hop++) {
      // for each hop except the first
      if (hop > 0) {
        float top = 0.0;
        float bottomLeft = 0.0;
        float bottomRight = 0.0;
        for (unsigned int band = 0; band < bands; band++) {
          // add a tiny amount to each magnitude to guard against div by zero
          float magA = ch.getMagnitude(hop - 1, band) + 0.001;
          float magB = ch.getMagnitude(hop, band) + 0.001;
          top += magA * magB;
          bottomLeft  += magA * magA;
          bottomRight += magB * magB;
        }
        cosineChanges[hop] = top / (sqrt(bottomLeft) * sqrt(bottomRight));
      }
      // invert similarity so it represents change instead
      cosineChanges[hop] = 1.0 - cosineChanges[hop];
    }
    // build gaussian kernel for smoothing
    WindowFunction win;
    std::vector<float> gaussian(gaussianSize);
    for (unsigned int i = 0; i < gaussianSize; i++) {
      gaussian[i] = win.gaussianWindow(i, gaussianSize, gaussianSigma);
    }
    // smooth change vector
    return win.convolve(cosineChanges, gaussian);
  }

}
