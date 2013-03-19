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

#include "chromatransformfactory.h"

namespace KeyFinder {

  ChromaTransformFactory::ChromaTransformWrapper::ChromaTransformWrapper(
    unsigned int f, const Parameters& p, ChromaTransform* s
  ) {
    frameRate = f;
    params = p;
    ct = s;
  }

  ChromaTransformFactory::ChromaTransformWrapper::~ChromaTransformWrapper() {
    delete ct;
  }

  ChromaTransform* ChromaTransformFactory::ChromaTransformWrapper::getChromaTransform() const {
    return ct;
  }

  Parameters ChromaTransformFactory::ChromaTransformWrapper::getParameters() const {
    return params;
  }

  unsigned int ChromaTransformFactory::ChromaTransformWrapper::getFrameRate() const {
    return frameRate;
  }

  ChromaTransformFactory::ChromaTransformFactory() {
    chromaTransforms = std::vector<ChromaTransformWrapper*>(0);
  }

  ChromaTransformFactory::~ChromaTransformFactory() {
    for (unsigned int i=0; i<chromaTransforms.size(); i++)
      delete chromaTransforms[i];
  }

  ChromaTransform* ChromaTransformFactory::getChromaTransform(
    unsigned int f, const Parameters& p
  ) {
    boost::mutex::scoped_lock lock(chromaTransformFactoryMutex);
    for (unsigned int i=0; i<chromaTransforms.size(); i++) {
      ChromaTransformWrapper* wrapper = chromaTransforms[i];
      if (
        wrapper->getFrameRate() == f &&
        p.equivalentSpectralKernels(wrapper->getParameters())
      ) {
        return wrapper->getChromaTransform();
      }
    }
    chromaTransforms.push_back(new ChromaTransformWrapper(f, p, new ChromaTransform(f, p)));
    return chromaTransforms[chromaTransforms.size()-1]->getChromaTransform();
  }

}
