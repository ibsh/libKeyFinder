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

#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include <QMutexLocker>

#include "chromagram.h"
#include "audiodata.h"
#include "parameters.h"

namespace KeyFinder{

  class SpectrumAnalyser{
  public:
    SpectrumAnalyser(unsigned int, const Parameters&);
    virtual Chromagram* chromagram(const AudioData&) = 0;
    virtual ~SpectrumAnalyser();
  protected:
    mutable QMutex analyserMutex; // used to make chromagram generation thread-safe
    unsigned int bins;
    unsigned int hopSize;
    unsigned int frameRate;
  };

} // namespace

#endif
