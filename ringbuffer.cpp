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

#include "ringbuffer.h"

namespace KeyFinder {

  RingBuffer::RingBuffer(unsigned int s) {
    if (s == 0) throw Exception("Size must be > 0");
    size = s;
    p = new Binode<float>(); // first node
    Binode<float>* q = p;
    for (unsigned int i = 0; i < size - 1; i++) {
      q->r = new Binode<float>(); // subsequent nodes
      q->r->l = q;
      q = q->r;
    }
    // join first and last nodes
    p->l = q;
    q->r = p;
  }

  RingBuffer::~RingBuffer() {
    Binode<float>* current = p;
    do {
      Binode<float>* zap = current;
      current = current->r;
      delete zap;
    } while (current != p);
  }

  unsigned int RingBuffer::getSize() const {
    return size;
  }

  void RingBuffer::clear() {
    Binode<float>* q = p;
    for (unsigned int k = 0; k < size; k++) {
      q->data = 0.0;
      q = q->r;
    }
  }

  void RingBuffer::shiftZeroIndex(int count) {
    if (count < 0) {
      for (int i = 0; i > count; i--)
        p = p->r;
    } else if (count > 0) {
      for (int i = 0; i < count; i++)
        p = p->l;
    }
  }

  float RingBuffer::getData(int index) const {
    Binode<float>* q = p;
    if (index < 0) {
      for (int i = 0; i > index; i--)
        q = q->l;
    } else if (index > 0) {
      for (int i = 0; i < index; i++)
        q = q->r;
    }
    return q->data;
  }

  void RingBuffer::setData(int index, float value) {
    Binode<float>* q = p;
    if (index < 0) {
      for (int i = 0; i > index; i--)
        q = q->l;
    } else if (index > 0) {
      for (int i = 0; i < index; i++)
        q = q->r;
    }
    q->data = value;
  }

}
