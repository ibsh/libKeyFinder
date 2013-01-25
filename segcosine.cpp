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

#include "segcosine.h"

namespace KeyFinder{

  std::vector<float> CosineHcdf::getRateOfChange(const Chromagram& ch, const Parameters& params){
    unsigned int hops = ch.getHops();
    unsigned int bins = ch.getBins();
    unsigned int gaussianSize = params.getSegGaussianSize();
    float gaussianSigma = params.getSegGaussianSigma();
    unsigned int padding = 0; // as opposed to gaussianSize/2
    std::vector<float> cosine(hops+padding, 0.0);
    for (unsigned int hop = 0; hop < hops; hop++){
      float top = 0.0;
      float bottom = 0.0;
      for (unsigned int bin = 0; bin < bins; bin++){
        float mag = ch.getMagnitude(hop, bin);
        top += mag;
        bottom += pow(mag,2);
      }
      float cos;
      if(bottom > 0.0) // divzero
        cos = top / sqrt(bottom) * sqrt(bins * sqrt(2));
      else
        cos = 0.0;
      cosine[hop] = cos;
    }
    // gaussian
    std::vector<float> gaussian(gaussianSize);
    for (unsigned int i=0; i<gaussianSize; i++){
      gaussian[i] = exp(-1 * (pow(i - gaussianSize/2 , 2) / (2 * gaussianSigma * gaussianSigma)));
    }
    std::vector<float> smoothed(hops);
    for (unsigned int hop = padding; hop < cosine.size(); hop++){
      float conv = 0.0;
      for (unsigned int k=0; k<gaussianSize; k++){
        int frm = hop - (gaussianSize/2) + k;
        if(frm >= 0 && frm < (signed)cosine.size()){
          conv += cosine[frm] * gaussian[k];
        }
      }
      smoothed[hop-padding] = conv;
    }
    // rate of change of hcdf signal; look at all hops except first.
    std::vector<float> rateOfChange(hops, 0.0);
    for (unsigned int hop=1; hop<hops; hop++){
      float change = (smoothed[hop] - smoothed[hop-1]) / 90.0;
      change = (change >= 0 ? change : change * -1.0);
      change /= 0.16; // TODO: fix awful magic number for display purposes in KeyFinder GUI app
      rateOfChange[hop] = change;
    }
    // fudge first
    rateOfChange[0] = rateOfChange[1];
    return rateOfChange;
  }

  std::vector<unsigned int> CosineHcdf::getSegments(const std::vector<float>& rateOfChange, const Parameters& params){
    // Pick peaks
    std::vector<unsigned int> changes(1, 0); // start vector with a 0 to enable first classification
    unsigned int neighbours = params.getSegPeakPickingNeighbours();
    for (unsigned int hop = 0; hop < rateOfChange.size(); hop++){
      bool peak = true;
      for (int i = -neighbours; i <= (signed)neighbours; i++)
        if(i != 0 && hop+i < rateOfChange.size()) // only test valid neighbours
          if(rateOfChange[hop+i] >= rateOfChange[hop])
            peak = false; // there's a neighbour of higher or equal value; this isn't a peak
      if(peak)
        changes.push_back(hop);
    }
    return changes;
  }

}
