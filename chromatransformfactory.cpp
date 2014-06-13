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
    unsigned int inFrameRate, const Parameters& inParams, const ChromaTransform* const inChromaTransform
  ) : frameRate(inFrameRate), params(inParams), chromaTransform(inChromaTransform) { }

  ChromaTransformFactory::ChromaTransformWrapper::~ChromaTransformWrapper() {
    delete chromaTransform;
  }

  const ChromaTransform* ChromaTransformFactory::ChromaTransformWrapper::getChromaTransform() const {
    return chromaTransform;
  }

  const Parameters& ChromaTransformFactory::ChromaTransformWrapper::getParameters() const {
    return params;
  }

  unsigned int ChromaTransformFactory::ChromaTransformWrapper::getFrameRate() const {
    return frameRate;
  }

  ChromaTransformFactory::ChromaTransformFactory() : chromaTransforms(0) { }

  ChromaTransformFactory::~ChromaTransformFactory() {
    for (unsigned int i = 0; i < chromaTransforms.size(); i++)
      delete chromaTransforms[i];
  }

  const ChromaTransform* ChromaTransformFactory::getChromaTransform(
    unsigned int frameRate, const Parameters& params
  ) {
    boost::mutex::scoped_lock lock(chromaTransformFactoryMutex);
    for (unsigned int i = 0; i < chromaTransforms.size(); i++) {
      ChromaTransformWrapper* wrapper = chromaTransforms[i];
      if (
        wrapper->getFrameRate() == frameRate &&
        params.equivalentSpectralKernels(wrapper->getParameters())
      ) {
        return wrapper->getChromaTransform();
      }
    }
    chromaTransforms.push_back(new ChromaTransformWrapper(frameRate, params, new ChromaTransform(frameRate, params)));
    return chromaTransforms[chromaTransforms.size()-1]->getChromaTransform();
  }

}
