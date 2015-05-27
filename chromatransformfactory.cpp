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

#include "chromatransformfactory.h"

namespace KeyFinder {

  ChromaTransformFactory::ChromaTransformWrapper::ChromaTransformWrapper(unsigned int inFrameRate, const ChromaTransform* const inChromaTransform) : frameRate(inFrameRate), chromaTransform(inChromaTransform) { }

  ChromaTransformFactory::ChromaTransformWrapper::~ChromaTransformWrapper() {
    delete chromaTransform;
  }

  const ChromaTransform* ChromaTransformFactory::ChromaTransformWrapper::getChromaTransform() const {
    return chromaTransform;
  }

  unsigned int ChromaTransformFactory::ChromaTransformWrapper::getFrameRate() const {
    return frameRate;
  }

  ChromaTransformFactory::ChromaTransformFactory() : chromaTransforms(0) { }

  ChromaTransformFactory::~ChromaTransformFactory() {
    for (unsigned int i = 0; i < chromaTransforms.size(); i++) {
      delete chromaTransforms[i];
    }
  }

  const ChromaTransform* ChromaTransformFactory::getChromaTransform(unsigned int frameRate) {
    for (unsigned int i = 0; i < chromaTransforms.size(); i++) {
      ChromaTransformWrapper* wrapper = chromaTransforms[i];
      if (wrapper->getFrameRate() == frameRate) {
        return wrapper->getChromaTransform();
      }
    }
    chromaTransformFactoryMutex.lock();
    chromaTransforms.push_back(new ChromaTransformWrapper(frameRate, new ChromaTransform(frameRate)));
    unsigned int newChromaTransformIndex = chromaTransforms.size()-1;
    chromaTransformFactoryMutex.unlock();
    return chromaTransforms[newChromaTransformIndex]->getChromaTransform();
  }

}
