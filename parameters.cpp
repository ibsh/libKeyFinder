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

#include "parameters.h"

namespace KeyFinder {

  bool                 Parameters::getOffsetToCDefault()                const { return true; }
  unsigned int         Parameters::getFftFrameSizeDefault()             const { return 16384; }
  unsigned int         Parameters::getHopsPerFrameDefault()             const { return 4; }
  unsigned int         Parameters::getHopSizeDefault()                  const { return getFftFrameSizeDefault() / getHopsPerFrameDefault(); }
  unsigned int         Parameters::getOctavesDefault()                  const { return 6; }
  unsigned int         Parameters::getBandsPerSemitoneDefault()         const { return 1; }
  unsigned int         Parameters::getBandsPerOctaveDefault()           const { return getBandsPerSemitoneDefault() * SEMITONES; }
  unsigned int         Parameters::getSegPeakPickingNeighboursDefault() const { return 4; }
  unsigned int         Parameters::getArbitrarySegmentsDefault()        const { return 3; }
  unsigned int         Parameters::getSegGaussianSizeDefault()          const { return 35; }
  float                Parameters::getSegGaussianSigmaDefault()         const { return 8.0; }
  float                Parameters::getStartingFreqADefault()            const { return 27.5; }
  float                Parameters::getDirectSkStretchDefault()          const { return 0.8; }
  float                Parameters::getDetunedBandWeightDefault()        const { return 0.2; }
  temporal_window_t    Parameters::getTemporalWindowDefault()           const { return WINDOW_BLACKMAN; }
  segmentation_t       Parameters::getSegmentationDefault()             const { return SEGMENTATION_NONE; }
  similarity_measure_t Parameters::getSimilarityMeasureDefault()        const { return SIMILARITY_COSINE; }
  tone_profile_t       Parameters::getToneProfileDefault()              const { return TONE_PROFILE_SHAATH; }
  tuning_method_t      Parameters::getTuningMethodDefault()             const { return TUNING_HARTE; }

  Parameters::Parameters() {
    // defaults
    offsetToC = getOffsetToCDefault();
    fftFrameSize = getFftFrameSizeDefault();
    hopsPerFrame = getHopsPerFrameDefault();
    octaves = getOctavesDefault();
    bps = getBandsPerSemitoneDefault();
    segPeakPickingNeighbours = getSegPeakPickingNeighboursDefault();
    arbitrarySegments = getArbitrarySegmentsDefault();
    segGaussianSize = getSegGaussianSizeDefault();
    segGaussianSigma = getSegGaussianSigmaDefault();
    stFreq = getStartingFreqADefault();
    directSkStretch = getDirectSkStretchDefault();
    detunedBandWeight = getDetunedBandWeightDefault();
    temporalWindow = getTemporalWindowDefault();
    segmentation = getSegmentationDefault();
    similarityMeasure = getSimilarityMeasureDefault();
    toneProfile = getToneProfileDefault();
    tuningMethod = getTuningMethodDefault();
    // a basic CTP
    float custom[24] = {
      1,0,1,0,1,1,0,1,0,1,0,1, // major
      1,0,1,1,0,1,0,1,1,0,1,0  // minor
    };
    customToneProfile = std::vector<float>(&custom[0], &custom[24]);
    // and other prep
    generateBandFreqs();
  }

  Parameters& Parameters::operator=(const Parameters& that) {
    if (this != &that) {
      temporalWindow = that.temporalWindow;
      segmentation = that.segmentation;
      fftFrameSize = that.fftFrameSize;
      hopsPerFrame = that.hopsPerFrame;
      octaves = that.octaves;
      bps = that.bps;
      offsetToC = that.offsetToC;
      toneProfile = that.toneProfile;
      similarityMeasure = that.similarityMeasure;
      segPeakPickingNeighbours = that.segPeakPickingNeighbours;
      arbitrarySegments = that.arbitrarySegments;
      segGaussianSize = that.segGaussianSize;
      tuningMethod = that.tuningMethod;
      segGaussianSigma = that.segGaussianSigma;
      stFreq = that.stFreq;
      directSkStretch = that.directSkStretch;
      detunedBandWeight = that.detunedBandWeight;
      customToneProfile = that.customToneProfile;
      generateBandFreqs();
    }
    return *this;
  }

  bool Parameters::equivalentSpectralKernels(const Parameters& that) const {
    if (stFreq != that.stFreq)
      return false;
    if (bps != that.bps)
      return false;
    if (octaves != that.octaves)
      return false;
    if (offsetToC != that.offsetToC)
      return false;
    if (fftFrameSize != that.fftFrameSize)
      return false;
    if (directSkStretch != that.directSkStretch)
      return false;
    return true;
  }

  // accessors
  bool                 Parameters::getOffsetToC()                const { return offsetToC; }
  temporal_window_t    Parameters::getTemporalWindow()           const { return temporalWindow; }
  segmentation_t       Parameters::getSegmentation()             const { return segmentation; }
  similarity_measure_t Parameters::getSimilarityMeasure()        const { return similarityMeasure; }
  tone_profile_t       Parameters::getToneProfile()              const { return toneProfile; }
  tuning_method_t      Parameters::getTuningMethod()             const { return tuningMethod; }
  unsigned int         Parameters::getFftFrameSize()             const { return fftFrameSize; }
  unsigned int         Parameters::getHopsPerFrame()             const { return hopsPerFrame; }
  unsigned int         Parameters::getHopSize()                  const { return fftFrameSize / hopsPerFrame; }
  unsigned int         Parameters::getOctaves()                  const { return octaves; }
  unsigned int         Parameters::getBandsPerSemitone()         const { return bps; }
  unsigned int         Parameters::getBandsPerOctave()           const { return bps * SEMITONES; }
  unsigned int         Parameters::getSegPeakPickingNeighbours() const { return segPeakPickingNeighbours; }
  unsigned int         Parameters::getArbitrarySegments()        const { return arbitrarySegments; }
  unsigned int         Parameters::getSegGaussianSize()          const { return segGaussianSize; }
  float                Parameters::getSegGaussianSigma()         const { return segGaussianSigma; }
  float                Parameters::getStartingFreqA()            const { return stFreq; }
  float                Parameters::getDirectSkStretch()          const { return directSkStretch; }
  float                Parameters::getDetunedBandWeight()        const { return detunedBandWeight; }
  std::vector<float>   Parameters::getCustomToneProfile()        const { return customToneProfile; }

  // basic mutators
  void Parameters::setTemporalWindow(temporal_window_t window)    { temporalWindow = window; }
  void Parameters::setSegmentation(segmentation_t f)              { segmentation = f; }
  void Parameters::setSimilarityMeasure(similarity_measure_t msr) { similarityMeasure = msr; }
  void Parameters::setToneProfile(tone_profile_t profile)         { toneProfile = profile; }
  void Parameters::setTuningMethod(tuning_method_t tune)          { tuningMethod = tune; }
  void Parameters::setSegPeakPickingNeighbours(unsigned int n)    { segPeakPickingNeighbours = n; }

  // mutators requiring validation or further work
  void Parameters::setOffsetToC(bool off) {
    offsetToC = off;
    generateBandFreqs();
  }
  void Parameters::setFftFrameSize(unsigned int framesize) {
    if (framesize < 1) throw Exception("FFT frame size must be > 0");
    fftFrameSize = framesize;
  }
  void Parameters::setHopsPerFrame(unsigned int hpf) {
    if (hpf < 1) throw Exception("Hops per frame must be > 0");
    hopsPerFrame = hpf;
  }
  void Parameters::setOctaves(unsigned int oct) {
    if (oct < 1) throw Exception("Octaves must be > 0");
    octaves = oct;
    generateBandFreqs();
  }
  void Parameters::setBandsPerSemitone(unsigned int bands) {
    if (bands < 1) throw Exception("Bands per semitone must be > 0");
    bps = bands;
    generateBandFreqs();
  }
  void Parameters::setArbitrarySegments(unsigned int s) {
    if (s < 1) throw Exception("Arbitrary segments must be > 0");
    arbitrarySegments = s;
  }
  void Parameters::setSegGaussianSize(unsigned int size) {
    if (size < 1) throw Exception("Gaussian size must be > 0");
    segGaussianSize = size;
  }
  void Parameters::setSegGaussianSigma(float sigma) {
    if (!boost::math::isfinite(sigma)) throw Exception("Gaussian sigma cannot be NaN");
    if (sigma <= 0) throw Exception("Gaussian sigma must be > 0");
    segGaussianSigma = sigma;
  }
  void Parameters::setStartingFrequencyA(float a) {
    if (
      a != 27.5  && a != 55.0  && a != 110.0  && a != 220.0  &&
      a != 440.0 && a != 880.0 && a != 1760.0 && a != 3520.0
    ) throw Exception("Starting frequency must be an A (2^n * 27.5 Hz)");
    stFreq = a;
    generateBandFreqs();
  }
  void Parameters::setDirectSkStretch(float stretch) {
    if (!boost::math::isfinite(stretch)) throw Exception("Spectral kernel stretch cannot be NaN");
    if (stretch <= 0) throw Exception("Spectral kernel stretch must be > 0");
    directSkStretch = stretch;
  }
  void Parameters::setDetunedBandWeight(float weight) {
    if (!boost::math::isfinite(weight)) throw Exception("Detuned band weighting cannot be NaN");
    if (weight < 0) throw Exception("Detuned band weighting must be >= 0");
    detunedBandWeight = weight;
  }
  void Parameters::setCustomToneProfile(const std::vector<float>& v) {
    if (v.size() != 24) throw Exception("Custom tone profile must have 24 elements");
    for (unsigned int i = 0; i < 24; i++)
      if (v[i] < 0)
        throw Exception("Custom tone profile elements must be >= 0");
    // Exception handling for occasional problem on OSX Leopard.
    try{
      customToneProfile = v;
    }catch(const std::exception& e) {
      throw Exception(e.what());
    }catch(...) {
      throw Exception("Unknown exception setting custom tone profile");
    }
  }

  void Parameters::generateBandFreqs() {
    unsigned int bpo = bps * SEMITONES;
    bandFreqs.clear();
    float freqRatio = pow(2, 1.0 / bpo);
    float octFreq = stFreq;
    float bandFreq;
    unsigned int concertPitchBin = bps/2;
    for (unsigned int i = 0; i < octaves; i++) {
      bandFreq = octFreq;
      // offset as required
      if (offsetToC) {
        bandFreq *= pow(freqRatio, 3);
      }
      // tune down for bins before first concert pitch bin (if bps > 1)
      for (unsigned int j = 0; j < concertPitchBin; j++) {
        bandFreqs.push_back(bandFreq / pow(freqRatio, concertPitchBin - j));
      }
      // and tune all other bins
      for (unsigned int j = concertPitchBin; j < bpo; j++) {
        bandFreqs.push_back(bandFreq);
        bandFreq *= freqRatio;
      }
      octFreq *= 2;
    }
  }

  float Parameters::getBandFrequency(unsigned int b) const {
    unsigned int max = octaves * SEMITONES * bps;
    if (b >= max) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds frequency index (" << b << "/" << max << ")";
      throw Exception(ss.str().c_str());
    }
    return bandFreqs[b];
  }

  float Parameters::getLastFrequency() const {
    return bandFreqs[bandFreqs.size()-1];
  }

}
