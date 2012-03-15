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

#ifndef SPECTRUMANALYSERFACTORY_H
#define SPECTRUMANALYSERFACTORY_H

#include <boost/thread/mutex.hpp>

#include "spectrumanalyser.h"
#include "spectrumanalyserfftw.h"
#include "parameters.h"

#include <vector>

namespace KeyFinder{

  // Keeps a reference to a spectrum analyser with distinguishing information
  class SpectrumAnalyserWrapper{
  public:
    SpectrumAnalyserWrapper(unsigned int, const Parameters&, SpectrumAnalyser*);
    ~SpectrumAnalyserWrapper();
    SpectrumAnalyser* getSpectrumAnalyser() const;
    Parameters chkParams() const;
    unsigned int chkFrameRate() const;
  private:
    unsigned int frate;
    Parameters params;
    SpectrumAnalyser* sa;
  };

  // Singleton. It holds all analysers generated in a session, to cut down on prep time.
  class SpectrumAnalyserFactory{
  public:
    SpectrumAnalyserFactory();
    ~SpectrumAnalyserFactory();
    SpectrumAnalyser* getSpectrumAnalyser(unsigned int, const Parameters&);
  private:
    std::vector<SpectrumAnalyserWrapper*> analysers;
    boost::mutex factoryMutex;
  };

} // namespace

#endif // SPECTRUMANALYSERFACTORY_H
