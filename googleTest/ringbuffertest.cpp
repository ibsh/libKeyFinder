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

#include "Ringbuffertest.h"

TEST (RingBufferTest, ConstructorWorks) {
  KeyFinder::RingBuffer b(10);
  ASSERT_EQ(10, b.getSize());
  for (unsigned int i = 0; i < 10; i++)
    ASSERT_FLOAT_EQ(0.0, b.getData(i));
}

TEST (RingBufferTest, CantInitialiseZeroLength) {
  ASSERT_THROW(KeyFinder::RingBuffer b(0), KeyFinder::Exception);
}

TEST (RingBufferTest, AccessorsMutatorsBasic) {
  unsigned int size = 5;
  KeyFinder::RingBuffer b(size);
  b.setData(1, 1.0);
  b.setData(3, 10.0);
  ASSERT_FLOAT_EQ(1.0, b.getData(1));
  ASSERT_FLOAT_EQ(10.0, b.getData(3));
}

TEST (RingBufferTest, AccessorsMutatorsUseCyclicalIndicesRelativeToZero) {
  unsigned int size = 5;
  KeyFinder::RingBuffer b(size);
  b.setData(0, 1.0);
  b.setData(3, 10.0);
  ASSERT_FLOAT_EQ(b.getData(0), b.getData(size));
  ASSERT_FLOAT_EQ(b.getData(0), b.getData(size * 2));
  ASSERT_FLOAT_EQ(b.getData(3), b.getData(-2));
  ASSERT_FLOAT_EQ(b.getData(3), b.getData(size + 3));
  ASSERT_FLOAT_EQ(b.getData(3), b.getData(size * 20 + 3));
  ASSERT_FLOAT_EQ(b.getData(3), b.getData(size * 20 -2));
}

TEST (RingBufferTest, Clear) {
  unsigned int size = 5;
  KeyFinder::RingBuffer b(size);
  b.setData(0, 1.0);
  b.setData(1, 5.0);
  b.setData(3, 10.0);
  b.clear();
  for (unsigned int i = 0; i < size; i++)
    ASSERT_FLOAT_EQ(0.0, b.getData(i));
}

TEST (RingBufferTest, ShiftZeroIndex) {
  unsigned int size = 5;
  KeyFinder::RingBuffer b(size);
  ASSERT_EQ(size, b.getSize());
  for (unsigned int i = 0; i < size; i++)
    ASSERT_FLOAT_EQ(0.0, b.getData(i));

  b.setData(0, 1.0);
  b.setData(3, 10.0);

  b.shiftZeroIndex(1);
  ASSERT_FLOAT_EQ(1.0, b.getData(1));
  ASSERT_FLOAT_EQ(10.0, b.getData(4));

  b.shiftZeroIndex(2);
  ASSERT_FLOAT_EQ(1.0, b.getData(3));
  ASSERT_FLOAT_EQ(10.0, b.getData(1));

  b.shiftZeroIndex(-1);
  ASSERT_FLOAT_EQ(1.0, b.getData(2));
  ASSERT_FLOAT_EQ(10.0, b.getData(0));

  b.shiftZeroIndex(-3);
  ASSERT_FLOAT_EQ(1.0, b.getData(4));
  ASSERT_FLOAT_EQ(10.0, b.getData(2));
}
