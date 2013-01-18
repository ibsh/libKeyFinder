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

#include "segharte.h"

namespace KeyFinder{

  float HarteHcdf::phi(int d, int l){
    unsigned int circle = d/2;
    float result = l * PI;
    float radius = 0.0;
    if(circle == 0){
      result *= (7.0/6.0);
      radius = 1.0;
    }else if(circle == 1){
      result *= (3.0/2.0);
      radius = 1.0;
    }else if(circle == 2){
      result *= (2.0/3.0);
      radius = 0.5;
    }
    if(d % 2 == 0){
      return (radius * sin(result));
    }else{
      return (radius * cos(result));
    }
  }

  std::vector<float> HarteHcdf::getRateOfChange(const Chromagram& ch, const Parameters& params){
    unsigned int bpo = 12; // note this function must always be applied *after* decomposition to 1 bps. TODO add validation.
    unsigned int hops = ch.getHops();
    unsigned int dims = 6;
    unsigned int gaussianSize = params.getHcdfGaussianSize();
    float gaussianSigma = params.getHcdfGaussianSigma();
    // build vector for movement in six dimensions
    // zero pad end of vector by half width of smoothing Gaussian? Haven't done that.
    unsigned int padding = 0; // as opposed to gaussianSize/2
    std::vector<std::vector<float> > zeta(hops+padding,std::vector<float>(dims));
    for (unsigned int hop=0; hop<hops; hop++){
      float taxicabNorm = 0.0;
      for (unsigned int bin=0; bin<bpo; bin++){
        taxicabNorm += (float)fabs(ch.getMagnitude(hop,bin));
      }
      for (unsigned int d=0; d<dims; d++){
        float sum = 0.0;
        for (unsigned int bin=0; bin<bpo; bin++){
          sum += (phi(d,bin) * (float)ch.getMagnitude(hop,bin));
        }
        float result = sum / taxicabNorm;
        if(result != result || result < -1.0 || result > 1.0)
          result = 0.0; // takes care of NaN and/or +/- infinity where track is close to silent.
        zeta[hop][d] = result;
      }
    }
    // gaussian
    std::vector<float> gaussian(gaussianSize);
    for (unsigned int i=0; i<gaussianSize; i++){
      // maybe. This is the def from http://en.wikipedia.org/wiki/Gaussian_function
      gaussian[i] = exp(-1 * (pow(i-gaussianSize/2,2) / (2 * gaussianSigma * gaussianSigma)));
      // there's also http://en.wikipedia.org/wiki/Window_function#Gaussian_windows
      // and http://www.mathworks.com/help/toolbox/signal/gausswin.html
    }
    // convolution with gaussian according to http://cnx.org/content/m11539/latest/
    std::vector<std::vector<float> > smoothed(hops,std::vector<float>(dims));
    for (unsigned int d = 0; d < dims; d++){
      for (unsigned int hop = padding; hop < zeta.size(); hop++){
        float conv = 0.0;
        for (unsigned int k = 0; k < gaussianSize; k++){
          int frm = hop - (gaussianSize/2) + k;
          if(frm >= 0 && frm < (signed)zeta.size()){
            conv += zeta[frm][d] * gaussian[k];
          }
        }
        smoothed[hop-padding][d] = conv;
      }
    }
    // rate of change of hcdf signal; look at all hops except first.
    std::vector<float> rateOfChange(hops);
    for (unsigned int hop=1; hop<hops; hop++){
      float xi = 0.0;
      for (unsigned int d=0; d<dims; d++){
        xi += pow((smoothed[hop][d] - smoothed[hop-1][d]),2);
      }
      xi = sqrt(xi);
      xi *= 3.0; // magic number; for display purposes only
      rateOfChange[hop] = xi;
    }
    // fudge first
    rateOfChange[0] = rateOfChange[1];
    return rateOfChange;
  }

  std::vector<unsigned int> HarteHcdf::getSegments(const std::vector<float>& rateOfChange, const Parameters& params){
    // Pick peaks
    std::vector<unsigned int> changes(1); // start vector with a 0 to enable first classification
    unsigned int neighbours = params.getHcdfPeakPickingNeighbours();
    for (unsigned int hop=0; hop<rateOfChange.size(); hop++){
      bool peak = true;
      for (int i =- (signed)neighbours; i <= (signed)neighbours; i++)
        if(i != 0 && hop+i < rateOfChange.size()) // only test valid neighbours
          if(rateOfChange[hop+i] >= rateOfChange[hop])
            peak = false; // there's a neighbour of higher or equal value; this isn't a peak
      if(peak)
        changes.push_back(hop);
    }
    return changes;
  }

}
