/*************************************************************************

  Copyright 2011-2015 Ibrahim Sha'ath

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

  ToneProfile::ToneProfile(const std::vector<double>& customProfile) {

    if (customProfile.size() != BANDS) {
      throw Exception("Tone profile must have 72 elements");
    }

    for (unsigned int o = 0; o < OCTAVES; o++) {
      Binode<double> *tonic = new Binode<double>((double)customProfile[o * SEMITONES]);
      Binode<double> *q = tonic;
      for (unsigned int i = 1; i<SEMITONES; i++) {
        q->r = new Binode<double>((double)customProfile[o * SEMITONES + i]);
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
      Binode<double>* p = tonics[o];
      do {
        Binode<double>* zap = p;
        p = p->r;
        delete zap;
      } while (p != tonics[o]);
    }
  }

  double ToneProfile::cosineSimilarity(const std::vector<double>& input, int offset) const {

    if (input.size() != BANDS) throw Exception("Chroma data must have 72 elements");

    double intersection = 0.0;
    double profileNorm = 0.0;
    double inputNorm = 0.0;

    for (unsigned int o = 0; o < OCTAVES; o++) {
      // Rotate starting pointer left for offset. Each step shifts the position
      // of the tonic one step further right of the starting pointer (or one semitone up).
      Binode<double>* p = tonics[o];
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
