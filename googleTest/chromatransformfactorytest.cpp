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

#include "chromatransformfactorytest.h"

TEST (ChromaTransformFactoryTest, RepeatedTransformRequests) {
  KeyFinder::ChromaTransformFactory ctf;

  KeyFinder::Parameters params;
  KeyFinder::ChromaTransform* ct1 = ctf.getChromaTransform(4410, params);
  KeyFinder::ChromaTransform* ct2 = ctf.getChromaTransform(4410, params);
  KeyFinder::ChromaTransform* ct3 = ctf.getChromaTransform(4800, params);
  params.setArbitrarySegments(9);
  KeyFinder::ChromaTransform* ct4 = ctf.getChromaTransform(4800, params);
  params.setOctaves(3);
  KeyFinder::ChromaTransform* ct5 = ctf.getChromaTransform(4800, params);

  ASSERT_EQ(ct1, ct2);
  ASSERT_NE(ct2, ct3);
  ASSERT_EQ(ct3, ct4);
  ASSERT_NE(ct4, ct5);
}
