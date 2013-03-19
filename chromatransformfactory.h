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

#ifndef CHROMATRANSFORMFACTORY_H
#define CHROMATRANSFORMFACTORY_H

#include <boost/thread/mutex.hpp>
#include <vector>
#include "chromatransform.h"
#include "parameters.h"

namespace KeyFinder {

  // Holds all analysers generated in a session, to cut down on prep time.
  class ChromaTransformFactory {
  public:
    ChromaTransformFactory();
    ~ChromaTransformFactory();
    ChromaTransform* getChromaTransform(
      unsigned int frameRate,
      const Parameters& params
    );
  private:
    class ChromaTransformWrapper;
    std::vector<ChromaTransformWrapper*> chromaTransforms;
    boost::mutex chromaTransformFactoryMutex;
  };

  // Keeps a reference to a spectrum analyser with distinguishing information
  class ChromaTransformFactory::ChromaTransformWrapper {
  public:
    ChromaTransformWrapper(
      unsigned int frameRate,
      const Parameters& params,
      ChromaTransform* transform
    );
    ~ChromaTransformWrapper();
    ChromaTransform* getChromaTransform() const;
    Parameters getParameters() const;
    unsigned int getFrameRate() const;
  private:
    unsigned int frameRate;
    Parameters params;
    ChromaTransform* ct;
  };

}

#endif
