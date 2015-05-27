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

#ifndef KEYFINDER_H
#define KEYFINDER_H

#include "audiodata.h"
#include "lowpassfilterfactory.h"
#include "chromatransformfactory.h"
#include "spectrumanalyser.h"
#include "keyclassifier.h"

namespace KeyFinder {

  class KeyFinder {
  public:

    // for progressive analysis
    void progressiveChromagram(AudioData audio, Workspace& workspace);
    void finalChromagram(Workspace& workspace);
    key_t keyOfChromagram(const Workspace& workspace) const;

    // for analysis of a whole audio file
    key_t keyOfAudio(const AudioData& audio);

    // for experimentation with alternative tone profiles
    key_t keyOfChromaVector(const std::vector<double>& chromaVector, const std::vector<double>& overrideMajorProfile, const std::vector<double>& overrideMinorProfile) const;

  private:
    void preprocess(AudioData& workingAudio, Workspace& workspace, bool flushRemainderBuffer = false);
    void chromagramOfBufferedAudio(Workspace& workspace);
    key_t keyOfChromaVector(const std::vector<double>& chromaVector) const;
    LowPassFilterFactory   lpfFactory;
    ChromaTransformFactory ctFactory;
    TemporalWindowFactory  twFactory;
  };

}

#endif
