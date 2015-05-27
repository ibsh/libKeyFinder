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

#include "keyclassifier.h"

namespace KeyFinder {

  KeyClassifier::KeyClassifier(const std::vector<double>& majorProfile, const std::vector<double>& minorProfile) {

    if (majorProfile.size() != BANDS) {
      throw Exception("Tone profile must have 72 elements");
    }

    if (minorProfile.size() != BANDS) {
      throw Exception("Tone profile must have 72 elements");
    }

    major   = new ToneProfile(majorProfile);
    minor   = new ToneProfile(minorProfile);
    silence = new ToneProfile(std::vector<double>(BANDS, 0.0));
  }

  KeyClassifier::~KeyClassifier() {
    delete major;
    delete minor;
    delete silence;
  }

  key_t KeyClassifier::classify(const std::vector<double>& chromaVector) {
    std::vector<double> scores(24);
    double bestScore = 0.0;
    for (unsigned int i = 0; i < SEMITONES; i++) {
      double score;
      score = major->cosineSimilarity(chromaVector, i); // major
      scores[i*2] = score;
      score = minor->cosineSimilarity(chromaVector, i); // minor
      scores[(i*2)+1] = score;
    }
    bestScore = silence->cosineSimilarity(chromaVector, 0);
    // find best match, defaulting to silence
    key_t bestMatch = SILENCE;
    for (unsigned int i = 0; i < 24; i++) {
      if (scores[i] > bestScore) {
        bestScore = scores[i];
        bestMatch = (key_t)i;
      }
    }
    return bestMatch;
  }

}
