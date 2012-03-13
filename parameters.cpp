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

#include "parameters.h"

namespace KeyFinder{

  Parameters::Parameters(){
    // defaults
    stFreq = 27.5;
    offsetToC = true;
    octaves = 6;
    bps = 1;
    temporalWindow = WINDOW_BLACKMAN;
    fftFrameSize = 16384;
    hopSize = fftFrameSize / 4;
    directSkStretch = 0.8;
    tuningMethod = TUNING_HARTE;
    detunedBandWeight = 0.2;
    segmentation = SEGMENTATION_NONE;
    hcdfGaussianSize = 35;
    hcdfGaussianSigma = 8.0;
    hcdfPeakPickingNeighbours = 4;
    hcdfArbitrarySegments = 3;
    toneProfile = TONE_PROFILE_SHAATH;
    similarityMeasure = SIMILARITY_COSINE;
    float custom[24] = {1,0,1,0,1,1,0,1,0,1,0,1, 1,0,1,1,0,1,0,1,1,0,1,0}; // major, minor
    customToneProfile = std::vector<float> (&custom[0], &custom[24]);
    // and other prep
    generateBinFreqs();
  }

  Parameters& Parameters::operator=(const Parameters& that){
    if(this != &that){
      temporalWindow = that.temporalWindow;
      segmentation = that.segmentation;
      hopSize = that.hopSize;
      fftFrameSize = that.fftFrameSize;
      octaves = that.octaves;
      bps = that.bps;
      offsetToC = that.offsetToC;
      toneProfile = that.toneProfile;
      similarityMeasure = that.similarityMeasure;
      hcdfPeakPickingNeighbours = that.hcdfPeakPickingNeighbours;
      hcdfArbitrarySegments = that.hcdfArbitrarySegments;
      hcdfGaussianSize = that.hcdfGaussianSize;
      tuningMethod = that.tuningMethod;
      hcdfGaussianSigma = that.hcdfGaussianSigma;
      stFreq = that.stFreq;
      directSkStretch = that.directSkStretch;
      detunedBandWeight = that.detunedBandWeight;
      customToneProfile = that.customToneProfile;
      generateBinFreqs();
    }
    return *this;
  }


  bool Parameters::equivalentForSpectralAnalysis(const Parameters& that) const{
    if(temporalWindow != that.temporalWindow)
      return false;
    if(bps != that.bps)
      return false;
    if(stFreq != that.stFreq)
      return false;
    if(octaves != that.octaves)
      return false;
    if(offsetToC != that.offsetToC)
      return false;
    if(fftFrameSize != that.fftFrameSize)
      return false;
    if(directSkStretch != that.directSkStretch)
      return false;
    return true;
  }

  bool                 Parameters::getOffsetToC()                 const { return offsetToC; }
  temporal_window_t    Parameters::getTemporalWindow()            const { return temporalWindow; }
  segmentation_t       Parameters::getSegmentation()              const { return segmentation; }
  similarity_measure_t Parameters::getSimilarityMeasure()         const { return similarityMeasure; }
  unsigned int         Parameters::getHopSize()                   const { return hopSize; }
  unsigned int         Parameters::getFftFrameSize()              const { return fftFrameSize; }
  unsigned int         Parameters::getOctaves()                   const { return octaves; }
  unsigned int         Parameters::getBpo()                       const { return bps * 12; }
  tone_profile_t       Parameters::getToneProfile()               const { return toneProfile; }
  tuning_method_t      Parameters::getTuningMethod()              const { return tuningMethod; }
  unsigned int         Parameters::getHcdfPeakPickingNeighbours() const { return hcdfPeakPickingNeighbours; }
  unsigned int         Parameters::getHcdfArbitrarySegments()     const { return hcdfArbitrarySegments; }
  unsigned int         Parameters::getHcdfGaussianSize()          const { return hcdfGaussianSize; }
  float                Parameters::getHcdfGaussianSigma()         const { return hcdfGaussianSigma; }
  float                Parameters::getStartingFreqA()             const { return stFreq; }
  float                Parameters::getDirectSkStretch()           const { return directSkStretch; }
  float                Parameters::getDetunedBandWeight()         const { return detunedBandWeight; }
  std::vector<float>   Parameters::getCustomToneProfile()         const { return customToneProfile; }

  void Parameters::setOffsetToC(bool off)                            { offsetToC = off; }
  void Parameters::setTemporalWindow(temporal_window_t window)       { temporalWindow = window; }
  void Parameters::setSegmentation(segmentation_t f)                 { segmentation = f; }
  void Parameters::setSimilarityMeasure(similarity_measure_t msr)    { similarityMeasure = msr; }
  void Parameters::setHopSize(unsigned int size)                     { hopSize = size; }
  void Parameters::setFftFrameSize(unsigned int framesize)           { fftFrameSize = framesize; }
  void Parameters::setOctaves(unsigned int oct)                      { octaves = oct; }
  void Parameters::setBps(unsigned int bands)                        { bps = bands; }
  void Parameters::setToneProfile(tone_profile_t profile)            { toneProfile = profile; }
  void Parameters::setHcdfPeakPickingNeighbours(unsigned int n)      { hcdfPeakPickingNeighbours = n; }
  void Parameters::setHcdfArbitrarySegments(unsigned int s)          { hcdfArbitrarySegments = s; }
  void Parameters::setHcdfGaussianSize(unsigned int size)            { hcdfGaussianSize = size; }
  void Parameters::setTuningMethod(tuning_method_t tune)             { tuningMethod = tune; }
  void Parameters::setHcdfGaussianSigma(float sigma)                 { hcdfGaussianSigma = sigma; }
  void Parameters::setStartingFreqA(float a)                         { stFreq = a; }
  void Parameters::setDirectSkStretch(float stretch)                 { directSkStretch = stretch; }
  void Parameters::setDetunedBandWeight(float weight)                { detunedBandWeight = weight; }
  void Parameters::setCustomToneProfile(const std::vector<float>& v) { customToneProfile = v; }

  float Parameters::getBinFreq(unsigned int n)const{
    if(n >= octaves*12*bps){
      std::cerr << "Attempt to get out-of-bounds frequency index (" << n << "/" << octaves*12*bps << ")" << std::endl;
      return 0;
    }
    return binFreqs[n];
  }

  float Parameters::getLastFreq() const{
    return binFreqs[binFreqs.size()-1];
  }

  void Parameters::generateBinFreqs(){
    unsigned int bpo = bps * 12;
    binFreqs.clear();
    float freqRatio = pow(2, 1.0 / bpo);
    float octFreq = stFreq;
    float binFreq;
    unsigned int concertPitchBin = bps/2;
    for (unsigned int i = 0; i < octaves; i++){
      binFreq = octFreq;
      // offset as required
      if(offsetToC){
        binFreq *= pow(freqRatio, 3);
      }
      // tune down for bins before first concert pitch bin (if bps > 1)
      for (unsigned int j = 0; j < concertPitchBin; j++)
        binFreqs.push_back(binFreq / pow(freqRatio, concertPitchBin - j));
      // and tune all other bins
      for (unsigned int j = concertPitchBin; j < bpo; j++){
        binFreqs.push_back(binFreq);
        binFreq *= freqRatio;
      }
      octFreq *= 2;
    }
  }

} // namespace
