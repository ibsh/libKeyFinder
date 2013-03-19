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

#ifndef LOWPASSFILTERFACTORY_H
#define LOWPASSFILTERFACTORY_H

#include <boost/thread/mutex.hpp>
#include <vector>
#include "lowpassfilter.h"
#include "parameters.h"

namespace KeyFinder {

  class LowPassFilterFactory {
  public:
    LowPassFilterFactory();
    ~LowPassFilterFactory();
    LowPassFilter* getLowPassFilter(
      unsigned int order,
      unsigned int frameRate,
      float cornerFrequency,
      unsigned int fftFrameSize
    );
  private:
    class LowPassFilterWrapper;
    std::vector<LowPassFilterWrapper*> filters;
    boost::mutex LowPassFilterFactoryMutex;
  };

  class LowPassFilterFactory::LowPassFilterWrapper {
  public:
    LowPassFilterWrapper(
      unsigned int order,
      unsigned int frameRate,
      float cornerFrequency,
      unsigned int fftFrameSize,
      LowPassFilter* filter
    );
    ~LowPassFilterWrapper();
    LowPassFilter* getLowPassFilter() const;
    unsigned int getOrder() const;
    unsigned int getFrameRate() const;
    float getCornerFrequency() const;
    unsigned int getFftFrameSize() const;
  private:
    unsigned int order;
    unsigned int frameRate;
    float cornerFrequency;
    unsigned int fftFrameSize;
    LowPassFilter* lpf;
  };

}

#endif
