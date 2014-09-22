/*************************************************************************

  Copyright 2011-2014 Ibrahim Sha'ath

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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <vector>
#include <deque>
#include <mutex>
#include "exception.h"

#undef  PI
#define PI 3.1415926535897932384626433832795

#undef SEMITONES
#define SEMITONES 12 // per octave, obviously

#undef OCTAVES
#define OCTAVES 6

#undef BANDS
#define BANDS SEMITONES * OCTAVES

#undef KEYS
#define KEYS SEMITONES * 2

#undef TONEPROFILESIZE
#define TONEPROFILESIZE BANDS * 2

#undef FFTFRAMESIZE
#define FFTFRAMESIZE 16384

#undef HOPSIZE
#define HOPSIZE FFTFRAMESIZE / 4

#undef DIRECTSKSTRETCH
#define DIRECTSKSTRETCH 0.8

static const unsigned int octaves = 6;

namespace KeyFinder {

  enum key_t {
    A_MAJOR,      A_MINOR,
    B_FLAT_MAJOR, B_FLAT_MINOR,
    B_MAJOR,      B_MINOR,
    C_MAJOR,      C_MINOR,
    D_FLAT_MAJOR, D_FLAT_MINOR,
    D_MAJOR,      D_MINOR,
    E_FLAT_MAJOR, E_FLAT_MINOR,
    E_MAJOR,      E_MINOR,
    F_MAJOR,      F_MINOR,
    G_FLAT_MAJOR, G_FLAT_MINOR,
    G_MAJOR,      G_MINOR,
    A_FLAT_MAJOR, A_FLAT_MINOR,
    SILENCE
  };

  enum temporal_window_t {
    WINDOW_BLACKMAN,
    WINDOW_HANN,
    WINDOW_HAMMING
  };

  enum scale_t {
    SCALE_MAJOR,
    SCALE_MINOR
  };

  float getFrequencyOfBand(unsigned int band);
  float getLastFrequency();
}

#endif
