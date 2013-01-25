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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#undef  PI
#define PI 3.1415926535897932384626433832795

namespace KeyFinder{

  enum key_t{
    A_MAJOR,       A_MINOR,
    B_FLAT_MAJOR,	 B_FLAT_MINOR,
    B_MAJOR,       B_MINOR,
    C_MAJOR,       C_MINOR,
    D_FLAT_MAJOR,	 D_FLAT_MINOR,
    D_MAJOR,       D_MINOR,
    E_FLAT_MAJOR,	 E_FLAT_MINOR,
    E_MAJOR,       E_MINOR,
    F_MAJOR,       F_MINOR,
    G_FLAT_MAJOR,	 G_FLAT_MINOR,
    G_MAJOR,       G_MINOR,
    A_FLAT_MAJOR,	 A_FLAT_MINOR,
    SILENCE
  };

  enum scale_t{
    SCALE_MAJOR,
    SCALE_MINOR
  };

  enum temporal_window_t{
    WINDOW_BLACKMAN,
    WINDOW_HANN,
    WINDOW_HAMMING
  };

  enum segmentation_t{
    SEGMENTATION_NONE,
    SEGMENTATION_ARBITRARY,
    SEGMENTATION_COSINE
  };

  enum similarity_measure_t{
    SIMILARITY_COSINE,
    SIMILARITY_CORRELATION
  };

  enum tone_profile_t{
    TONE_PROFILE_SILENCE,
    TONE_PROFILE_KRUMHANSL,
    TONE_PROFILE_TEMPERLEY,
    TONE_PROFILE_GOMEZ,
    TONE_PROFILE_SHAATH,
    TONE_PROFILE_CUSTOM
  };

  enum tuning_method_t{
    TUNING_HARTE,
    TUNING_BIN_ADAPTIVE
  };

}

#endif
