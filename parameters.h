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

#include <cmath>
#include <vector>
#include <boost/math/special_functions/fpclassify.hpp>
#include "constants.h"
#include "exception.h"

namespace KeyFinder{

  class Parameters{
  public:

    Parameters();
    Parameters& operator=(const Parameters&);
    bool equivalentSpectralKernels(const Parameters&) const;

    // getters
    bool getOffsetToC() const;
    unsigned int getFftFrameSize() const;
    unsigned int getHopsPerFrame() const;
    unsigned int getHopSize() const;
    unsigned int getOctaves() const;
    unsigned int getBandsPerOctave() const;
    unsigned int getArbitrarySegments() const;
    unsigned int getSegPeakPickingNeighbours() const;
    unsigned int getSegGaussianSize() const;
    float getSegGaussianSigma() const;
    float getStartingFreqA() const;
    float getBinFrequency(unsigned int bin) const;
    float getLastFrequency() const;
    float getDirectSkStretch() const;
    float getDetunedBandWeight() const;
    temporal_window_t getTemporalWindow() const;
    segmentation_t getSegmentation() const;
    similarity_measure_t getSimilarityMeasure() const;
    tone_profile_t getToneProfile() const;
    tuning_method_t getTuningMethod() const;
    std::vector<float> getCustomToneProfile() const;

    // setters
    void setOffsetToC(bool offsetToC);
    void setFftFrameSize(unsigned int fftFrameSize);
    void setHopsPerFrame(unsigned int hopsPerFrame);
    void setOctaves(unsigned int octaves);
    void setBandsPerSemitone(unsigned int bandsPerSemitone);
    void setArbitrarySegments(unsigned int arbitrarySegments);
    void setSegPeakPickingNeighbours(unsigned int segPeakPickingNeighbours);
    void setSegGaussianSize(unsigned int segGaussianSize);
    void setSegGaussianSigma(float segGaussianSigma);
    void setStartingFrequencyA(float startingFrequency);
    void setDirectSkStretch(float directSkStretch);
    void setDetunedBandWeight(float detunedBandWeight);
    void setTemporalWindow(temporal_window_t temporalWindow);
    void setSegmentation(segmentation_t segmentation);
    void setSimilarityMeasure(similarity_measure_t similarityMeasure);
    void setToneProfile(tone_profile_t toneProfile);
    void setTuningMethod(tuning_method_t tuningMethod);
    void setCustomToneProfile(const std::vector<float>& customToneProfile);

  private:
    bool offsetToC;
    unsigned int fftFrameSize;
    unsigned int hopsPerFrame;
    unsigned int octaves;
    unsigned int bps;
    unsigned int arbitrarySegments;
    unsigned int segPeakPickingNeighbours;
    unsigned int segGaussianSize;
    float segGaussianSigma;
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
