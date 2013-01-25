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

#include "keyclassifier.h"

namespace KeyFinder{

  KeyClassifier::KeyClassifier(const Parameters& params){
    // Profiles
    major   = new ToneProfile(params.getToneProfile(), SCALE_MAJOR, params);
    minor   = new ToneProfile(params.getToneProfile(), SCALE_MINOR, params);
    silence = new ToneProfile(TONE_PROFILE_SILENCE,    SCALE_MAJOR, params);
    similarityMeasure = params.getSimilarityMeasure();
  }

  KeyClassifier::~KeyClassifier(){
    delete major;
    delete minor;
    delete silence;
  }

  key_t KeyClassifier::classify(const std::vector<float>& chroma){
    std::vector<float> scores(24);
    float bestScore = 0.0;
    float chromaMean = 0.0;
    for (unsigned int i=0; i<chroma.size(); i++)
      chromaMean += chroma[i];
    chromaMean /= chroma.size();
    for (unsigned int i=0; i<12; i++){
      float score;
      score = major->similarity(similarityMeasure, chroma, i, chromaMean); // major
      scores[i*2] = score;
      score = minor->similarity(similarityMeasure, chroma, i, chromaMean); // minor
      scores[(i*2)+1] = score;
    }
    bestScore = silence->similarity(similarityMeasure, chroma, 0, chromaMean);
    // find best match, defaulting to silence
    key_t bestMatch = SILENCE;
    for (unsigned int i=0; i<24; i++){
      if(scores[i] > bestScore){
        bestScore = scores[i];
        bestMatch = (key_t)i;
      }
    }
    return bestMatch;
  }

}
