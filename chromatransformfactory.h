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

#ifndef CHROMATRANSFORMFACTORY_H
#define CHROMATRANSFORMFACTORY_H

#include "constants.h"
#include "chromatransform.h"

namespace KeyFinder {

  class ChromaTransformFactory {
  public:
    ChromaTransformFactory();
    ~ChromaTransformFactory();
    const ChromaTransform* getChromaTransform(unsigned int frameRate);
  private:
    class ChromaTransformWrapper;
    std::vector<ChromaTransformWrapper*> chromaTransforms;
    std::mutex chromaTransformFactoryMutex;
  };

  class ChromaTransformFactory::ChromaTransformWrapper {
  public:
    ChromaTransformWrapper(unsigned int frameRate, const ChromaTransform* const transform);
    ~ChromaTransformWrapper();
    const ChromaTransform* getChromaTransform() const;
    unsigned int getFrameRate() const;
  private:
    unsigned int frameRate;
    const ChromaTransform* const chromaTransform;
  };

}

#endif
