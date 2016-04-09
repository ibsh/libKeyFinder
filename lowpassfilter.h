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

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include "constants.h"
#include "audiodata.h"
#include "workspace.h"

namespace KeyFinder {

  class LowPassFilterPrivate;

  class LowPassFilter {
  public:
    LowPassFilter(unsigned int order, unsigned int frameRate, double cornerFrequency, unsigned int fftFrameSize);
    ~LowPassFilter();
    void filter(AudioData& audio, Workspace& workspace, unsigned int shortcutFactor = 1) const;
    void const * getCoefficients() const; // for unit testing only
  protected:
    LowPassFilterPrivate* priv;
  };

}

#endif
