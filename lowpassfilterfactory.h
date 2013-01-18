/*************************************************************************

  Copyright 2012 Ibrahim Sha'ath

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

namespace KeyFinder{

  class LowPassFilterWrapper{
  public:
    LowPassFilterWrapper(unsigned int, unsigned int, float, unsigned int, LowPassFilter*);
    ~LowPassFilterWrapper();
    LowPassFilter* getLowPassFilter() const;
    unsigned int chkCoefficientCount() const;
    unsigned int chkFrameRate() const;
    float chkCornerFrequency() const;
    unsigned int chkFftFrameSize() const;
  private:
    unsigned int coefficientCount;
    unsigned int frameRate;
    float cornerFrequency;
    unsigned int fftFrameSize;
    LowPassFilter* lpf;
  };

  class LowPassFilterFactory{
  public:
    LowPassFilterFactory();
    ~LowPassFilterFactory();
    LowPassFilter* getLowPassFilter(unsigned int, unsigned int, float, unsigned int);
  private:
    std::vector<LowPassFilterWrapper*> filters;
    boost::mutex LowPassFilterFactoryMutex;
  };

}

#endif
