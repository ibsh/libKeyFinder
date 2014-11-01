/*************************************************************************

  Copyright 2011-2014 Ibrahim Sha'ath

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

#include "lowpassfilterfactory.h"

namespace KeyFinder {

  LowPassFilterFactory::LowPassFilterWrapper::LowPassFilterWrapper(
    unsigned int inOrder,
    unsigned int inFrameRate,
    double inCornerFrequency,
    unsigned int inFftFrameSize,
    const LowPassFilter* const inLowPassFilter
  ) {
    order = inOrder;
    frameRate = inFrameRate;
    cornerFrequency = inCornerFrequency;
    fftFrameSize = inFftFrameSize;
    lowPassFilter = inLowPassFilter;
  }

  LowPassFilterFactory::LowPassFilterWrapper::~LowPassFilterWrapper() {
    delete lowPassFilter;
  }

  const LowPassFilter* LowPassFilterFactory::LowPassFilterWrapper::getLowPassFilter() const {
    return lowPassFilter;
  }

  unsigned int LowPassFilterFactory::LowPassFilterWrapper::getOrder() const {
    return order;
  }

  unsigned int LowPassFilterFactory::LowPassFilterWrapper::getFrameRate() const {
    return frameRate;
  }

  double LowPassFilterFactory::LowPassFilterWrapper::getCornerFrequency() const {
    return cornerFrequency;
  }

  unsigned int LowPassFilterFactory::LowPassFilterWrapper::getFftFrameSize() const {
    return fftFrameSize;
  }

  LowPassFilterFactory::LowPassFilterFactory() : filters(0) { }

  LowPassFilterFactory::~LowPassFilterFactory() {
    for (unsigned int i = 0; i < filters.size(); i++)
      delete filters[i];
  }

  const LowPassFilter* LowPassFilterFactory::getLowPassFilter(
    unsigned int inOrder, unsigned int inFrameRate, double inCornerFrequency, unsigned int inFftFrameSize
  ) {
    lowPassFilterFactoryMutex.lock();
    for (unsigned int i = 0; i < filters.size(); i++) {
      LowPassFilterWrapper* wrapper = filters[i];
      if (
        wrapper->getOrder() == inOrder &&
        wrapper->getFrameRate() == inFrameRate &&
        wrapper->getCornerFrequency() == inCornerFrequency &&
        wrapper->getFftFrameSize() == inFftFrameSize
      ) {
        return wrapper->getLowPassFilter();
      }
    }
    filters.push_back(
      new LowPassFilterWrapper(
        inOrder, inFrameRate, inCornerFrequency, inFftFrameSize, new LowPassFilter(inOrder, inFrameRate, inCornerFrequency, inFftFrameSize)
      )
    );
    lowPassFilterFactoryMutex.unlock();
    return filters[filters.size()-1]->getLowPassFilter();
  }

}
