/*************************************************************************

  Copyright 2012 Ibrahim Sha'ath

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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>
#include <sstream>
#include <vector>
#include "exception.h"

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

  enum temporal_window_t{
    WINDOW_BLACKMAN,
    WINDOW_HANN,
    WINDOW_HAMMING
  };

  enum segmentation_t{
    SEGMENTATION_NONE,
    SEGMENTATION_ARBITRARY,
    SEGMENTATION_COSINE,
    SEGMENTATION_HARTE
  };

  enum similarity_measure_t{
    SIMILARITY_COSINE,
    SIMILARITY_CORRELATION
  };

  enum tone_profile_t{
    TONE_PROFILE_SILENT,
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

  class Parameters{
  public:

    Parameters();
    Parameters& operator=(const Parameters&);
    bool equivalentForSpectralAnalysis(const Parameters&) const;

    // getters
    bool getOffsetToC() const;
    unsigned int getHopSize() const;
    unsigned int getFftFrameSize() const;
    unsigned int getOctaves() const;
    unsigned int getBpo() const;
    unsigned int getArbitrarySegments() const;
    unsigned int getHcdfPeakPickingNeighbours() const;
    unsigned int getHcdfGaussianSize() const;
    float getHcdfGaussianSigma() const;
    float getStartingFreqA() const;
    float getBinFreq(unsigned int) const;
    float getLastFreq() const;
    float getDirectSkStretch() const;
    float getDetunedBandWeight() const;
    temporal_window_t getTemporalWindow() const;
    segmentation_t getSegmentation() const;
    similarity_measure_t getSimilarityMeasure() const;
    tone_profile_t getToneProfile() const;
    tuning_method_t getTuningMethod() const;
    std::vector<float> getCustomToneProfile() const;

    // setters
    void setOffsetToC(bool);
    void setHopSize(unsigned int);
    void setFftFrameSize(unsigned int);
    void setOctaves(unsigned int);
    void setBps(unsigned int);
    void setArbitrarySegments(unsigned int);
    void setHcdfPeakPickingNeighbours(unsigned int);
    void setHcdfGaussianSize(unsigned int);
    void setHcdfGaussianSigma(float);
    void setStartingFreqA(float);
    void setDirectSkStretch(float);
    void setDetunedBandWeight(float);
    void setTemporalWindow(temporal_window_t);
    void setSegmentation(segmentation_t);
    void setSimilarityMeasure(similarity_measure_t);
    void setToneProfile(tone_profile_t);
    void setTuningMethod(tuning_method_t);
    void setCustomToneProfile(const std::vector<float>&);

  private:
    bool offsetToC;
    unsigned int hopSize;
    unsigned int fftFrameSize;
    unsigned int octaves;
    unsigned int bps;
    unsigned int arbitrarySegments;
    unsigned int hcdfPeakPickingNeighbours;
    unsigned int hcdfGaussianSize;
    float hcdfGaussianSigma;
    float stFreq;
    float directSkStretch;
    float detunedBandWeight;
    temporal_window_t temporalWindow;
    segmentation_t segmentation;
    similarity_measure_t similarityMeasure;
    tone_profile_t toneProfile;
    tuning_method_t tuningMethod;
    std::vector<float> binFreqs;
    std::vector<float> customToneProfile;

    void generateBinFreqs();
  };

} // namespace

#endif
