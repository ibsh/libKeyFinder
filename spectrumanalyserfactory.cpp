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

#include "spectrumanalyserfactory.h"

namespace KeyFinder{

  SpectrumAnalyserWrapper::SpectrumAnalyserWrapper(unsigned int f, const Parameters& p, SpectrumAnalyser* s){
    frate = f;
    params = p;
    sa = s;
  }

  SpectrumAnalyserWrapper::~SpectrumAnalyserWrapper(){
    delete sa;
  }

  SpectrumAnalyser* SpectrumAnalyserWrapper::getSpectrumAnalyser() const{
    return sa;
  }

  Parameters SpectrumAnalyserWrapper::chkParams() const{
    return params;
  }

  unsigned int SpectrumAnalyserWrapper::chkFrameRate() const{
    return frate;
  }

  SpectrumAnalyserFactory::SpectrumAnalyserFactory(){
    analysers = std::vector<SpectrumAnalyserWrapper*>(0);
  }

  SpectrumAnalyserFactory::~SpectrumAnalyserFactory(){
    for (unsigned int i=0; i<analysers.size(); i++)
      delete analysers[i];
  }

  SpectrumAnalyser* SpectrumAnalyserFactory::getSpectrumAnalyser(unsigned int f, const Parameters& p){
    QMutexLocker locker(&factoryMutex); // This function should be accessed by only one thread at a time
    for (unsigned int i=0; i<analysers.size(); i++)
      if(analysers[i]->chkFrameRate() == f && p.equivalentForSpectralAnalysis(analysers[i]->chkParams()))
        return analysers[i]->getSpectrumAnalyser();
    // no match found, build a new spectrum analyser
    analysers.push_back(new SpectrumAnalyserWrapper(f, p, new FftwAnalyser(f, p)));
    return analysers[analysers.size()-1]->getSpectrumAnalyser();
  }

} // namespace
