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

#include "workspacetest.h"

TEST (WorkspaceTest, ConstructorDefaultsWork) {
  KeyFinder::Workspace w;

  ASSERT_EQ(0, w.buffer.getChannels());
  ASSERT_EQ(0, w.buffer.getFrameRate());
  ASSERT_EQ(0, w.buffer.getSampleCount());

  ASSERT_EQ(NULL, w.chroma);
  ASSERT_EQ(NULL, w.getLpfBuffer());
  ASSERT_EQ(NULL, w.getFftAdapter());
}

TEST (WorkspaceTest, FftAdapterCanOnlyBeSetOnce) {
  KeyFinder::Workspace w;
  KeyFinder::FftAdapter* f = new KeyFinder::FftAdapter(16384);
  ASSERT_NO_THROW(w.setFftAdapter(f));
  ASSERT_EQ(f, w.getFftAdapter());
  ASSERT_THROW(w.setFftAdapter(f), KeyFinder::Exception);
}

TEST (WorkspaceTest, LpfBufferConstructorRequiresPositiveImpulseLength) {
  KeyFinder::Workspace w;
  ASSERT_THROW(w.constructLpfBuffer(0), KeyFinder::Exception);
}

TEST (WorkspaceTest, LpfBufferCanOnlyBeConstructedOnce) {
  KeyFinder::Workspace w;
  ASSERT_NO_THROW(w.constructLpfBuffer(10));
  unsigned int count = 1;
  KeyFinder::Binode<float>* p = w.getLpfBuffer()->r;
  for (; p != w.getLpfBuffer(); count++) {
    p = p->r;
  }
  ASSERT_EQ(10, count);
  ASSERT_THROW(w.constructLpfBuffer(1), KeyFinder::Exception);
}

TEST (WorkspaceTest, DestructorIsSafeBeforeFftAdapterInitialisation) {
  KeyFinder::Workspace* w;
  w = new KeyFinder::Workspace();
  ASSERT_NO_THROW(delete w);
}

TEST (WorkspaceTest, DestructorIsSafeAfterFftAdapterInitialisation) {
  KeyFinder::Workspace* w;
  w = new KeyFinder::Workspace();
  w->setFftAdapter(new KeyFinder::FftAdapter(16384));
  ASSERT_NO_THROW(delete w);
}
