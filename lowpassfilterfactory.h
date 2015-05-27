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

#ifndef LOWPASSFILTERFACTORY_H
#define LOWPASSFILTERFACTORY_H

#include "constants.h"
#include "lowpassfilter.h"

namespace KeyFinder {

  class LowPassFilterFactory {
  public:
    LowPassFilterFactory();
    ~LowPassFilterFactory();
    const LowPassFilter* getLowPassFilter(unsigned int order, unsigned int frameRate, double cornerFrequency, unsigned int fftFrameSize);
  private:
    class LowPassFilterWrapper;
    std::vector<LowPassFilterWrapper*> lowPassFilters;
    std::mutex lowPassFilterFactoryMutex;
  };

  class LowPassFilterFactory::LowPassFilterWrapper {
  public:
    LowPassFilterWrapper(unsigned int order, unsigned int frameRate, double cornerFrequency, unsigned int fftFrameSize, const LowPassFilter* const filter);
    ~LowPassFilterWrapper();
    const LowPassFilter* getLowPassFilter() const;
    unsigned int getOrder() const;
    unsigned int getFrameRate() const;
    double getCornerFrequency() const;
    unsigned int getFftFrameSize() const;
  private:
    unsigned int order;
    unsigned int frameRate;
    double cornerFrequency;
    unsigned int fftFrameSize;
    const LowPassFilter* lowPassFilter;
  };

}

#endif
