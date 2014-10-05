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

#include "toneprofiles.h"

namespace KeyFinder {

  ToneProfile::ToneProfile(const std::vector<float>& customProfile) {

    if (customProfile.size() != BANDS) throw Exception("Tone profile must have 72 elements");

    for (unsigned int o = 0; o < OCTAVES; o++) {
      // copy into doubly-linked circular list
      Binode<float> *tonic = new Binode<float>((float)customProfile[o * SEMITONES]);
      Binode<float> *q = tonic;
      for (unsigned int i = 1; i<SEMITONES; i++) {
        q->r = new Binode<float>((float)customProfile[o * SEMITONES + i]);
        q->r->l = q;
        q = q->r;
      }
      q->r = tonic;
      tonic->l = q;

      // offset from A to C (3 semitones)
      for (unsigned int i=0; i<3; i++) {
        tonic = tonic->r;
      }

      tonics.push_back(tonic);
    }
  }

  ToneProfile::~ToneProfile() {
    free();
  }

  void ToneProfile::free() {
    for (unsigned int o = 0; o < OCTAVES; o++) {
      Binode<float>* p = tonics[o];
      do {
        Binode<float>* zap = p;
        p = p->r;
        delete zap;
      } while (p != tonics[o]);
    }
  }

  float ToneProfile::cosineSimilarity(const std::vector<float>& input, int offset) const {

    if (input.size() != BANDS) throw Exception("Chroma data must have 72 elements");

    float intersection = 0.0;
    float profileNorm = 0.0;
    float inputNorm = 0.0;

    for (unsigned int o = 0; o < OCTAVES; o++) {
    // Rotate starting pointer left for offset. Each step shifts the position
    // of the tonic one step further right of the starting pointer (or one semitone up).
      Binode<float>* p = tonics[o];
      for (int i=0; i<offset; i++) {
        p = p->l;
      }
      for (unsigned int i = o * SEMITONES; i < (o + 1) * SEMITONES; i++) {
        intersection += input[i] * p->data;
        profileNorm += pow((p->data),2);
        inputNorm += pow((input[i]),2);
        p = p->r;
      }
    }

    if (profileNorm > 0 && inputNorm > 0) {
      // div by zero check
      return intersection / (sqrt(profileNorm) * sqrt(inputNorm));
    } else {
      return 0;
    }
  }
}
