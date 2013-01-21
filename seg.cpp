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

#include "seg.h"

#include "segcosine.h"
#include "segnone.h"
#include "segarbitrary.h"

namespace KeyFinder{

  Segmentation* Segmentation::getSegmentation(const Parameters& params){
    if(params.getSegmentation() == SEGMENTATION_COSINE)
      return new CosineHcdf();
    else if(params.getSegmentation() == SEGMENTATION_ARBITRARY)
      return new ArbitrarySeg();
    else
      return new NoSeg();
  }

  Segmentation::~Segmentation(){ }

}
