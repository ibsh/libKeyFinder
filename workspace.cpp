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

#include "workspace.h"

namespace KeyFinder {

Workspace::Workspace() :
  buffer(), chroma(NULL), fftAdapter(NULL), lpfBuffer(NULL) { }

  Workspace::~Workspace() {
    if (fftAdapter != NULL)
      delete fftAdapter;
    if (chroma != NULL)
      delete chroma;
    if (lpfBuffer != NULL) {
      Binode<float>* current = lpfBuffer;
      do {
        Binode<float>* zap = current;
        current = current->r;
        delete zap;
      } while (current != lpfBuffer);
    }
  }

  FftAdapter* Workspace::getFftAdapter() {
    return fftAdapter;
  }

  void Workspace::setFftAdapter(FftAdapter* const fft) {
    if (fftAdapter != NULL)
      throw Exception("Can only set FFT adapter pointer once");
    fftAdapter = fft;
  }

  Binode<float>* Workspace::getLpfBuffer() {
    return lpfBuffer;
  }

  void Workspace::constructLpfBuffer(unsigned int impulseLength) {
    if (lpfBuffer != NULL)
      throw Exception("Can only construct LPF buffer once");
    if (impulseLength < 1) throw Exception("Impulse length must be > 0");
    // first node
    Binode<float>* p = new Binode<float>();
    Binode<float>* q = p;
    // subsequent nodes, for a total of impulseLength
    for (unsigned int i = 0; i < impulseLength - 1; i++) {
      q->r = new Binode<float>();
      q->r->l = q;
      q = q->r;
    }
    // join first and last nodes
    p->l = q;
    q->r = p;
    lpfBuffer = p;
  }

}
