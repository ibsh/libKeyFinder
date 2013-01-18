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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>
#include <sstream>
#include <vector>
#include "constants.h"
#include "exception.h"

namespace KeyFinder{

  class Parameters{
  public:

    Parameters();
    Parameters& operator=(const Parameters&);
    bool equivalentForSpectralAnalysis(const Parameters&) const;

    // getters
    bool getOffsetToC() const;
    unsigned int getFftFrameSize() const;
    unsigned int getHopsPerFrame() const;
    unsigned int getHopSize() const;
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
    void setFftFrameSize(unsigned int);
    void setHopsPerFrame(unsigned int);
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
    unsigned int fftFrameSize;
    unsigned int hopsPerFrame;
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

}

#endif
