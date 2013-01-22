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

namespace KeyFinder{

  Parameters::Parameters(){
    // defaults
    stFreq = 27.5;
    offsetToC = true;
    octaves = 6;
    bps = 1;
    temporalWindow = WINDOW_BLACKMAN;
    fftFrameSize = 16384;
    hopsPerFrame = 4;
    directSkStretch = 0.8;
    tuningMethod = TUNING_HARTE;
    detunedBandWeight = 0.2;
    segmentation = SEGMENTATION_NONE;
    segGaussianSize = 35;
    segGaussianSigma = 8.0;
    segPeakPickingNeighbours = 4;
    arbitrarySegments = 3;
    toneProfile = TONE_PROFILE_SHAATH;
    similarityMeasure = SIMILARITY_COSINE;
    float custom[24] = {
      1,0,1,0,1,1,0,1,0,1,0,1, // major
      1,0,1,1,0,1,0,1,1,0,1,0  // minor
    };
    customToneProfile = std::vector<float> (&custom[0], &custom[24]);
    // and other prep
    generateBinFreqs();
  }

  Parameters& Parameters::operator=(const Parameters& that){
    if(this != &that){
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
      generateBinFreqs();
    }
    return *this;
  }

  bool Parameters::equivalentSpectralKernels(const Parameters& that) const{
    if(stFreq != that.stFreq)
      return false;
    if(bps != that.bps)
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
  unsigned int         Parameters::getFftFrameSize()              const { return fftFrameSize; }
  unsigned int         Parameters::getHopsPerFrame()              const { return hopsPerFrame; }
  unsigned int         Parameters::getHopSize()                   const { return fftFrameSize / hopsPerFrame; }
  unsigned int         Parameters::getOctaves()                   const { return octaves; }
  unsigned int         Parameters::getBpo()                       const { return bps * 12; }
  tone_profile_t       Parameters::getToneProfile()               const { return toneProfile; }
  tuning_method_t      Parameters::getTuningMethod()              const { return tuningMethod; }
  unsigned int         Parameters::getSegPeakPickingNeighbours() const { return segPeakPickingNeighbours; }
  unsigned int         Parameters::getArbitrarySegments()         const { return arbitrarySegments; }
  unsigned int         Parameters::getSegGaussianSize()          const { return segGaussianSize; }
  float                Parameters::getSegGaussianSigma()         const { return segGaussianSigma; }
  float                Parameters::getStartingFreqA()             const { return stFreq; }
  float                Parameters::getDirectSkStretch()           const { return directSkStretch; }
  float                Parameters::getDetunedBandWeight()         const { return detunedBandWeight; }
  std::vector<float>   Parameters::getCustomToneProfile()         const { return customToneProfile; }

  // basic mutators
  void Parameters::setTemporalWindow(temporal_window_t window)    { temporalWindow = window; }
  void Parameters::setSegmentation(segmentation_t f)              { segmentation = f; }
  void Parameters::setSimilarityMeasure(similarity_measure_t msr) { similarityMeasure = msr; }
  void Parameters::setToneProfile(tone_profile_t profile)         { toneProfile = profile; }
  void Parameters::setSegPeakPickingNeighbours(unsigned int n)   { segPeakPickingNeighbours = n; }
  void Parameters::setTuningMethod(tuning_method_t tune)          { tuningMethod = tune; }

  // mutators requiring validation or further work
  void Parameters::setOffsetToC(bool off){
    offsetToC = off;
    generateBinFreqs();
  }
  void Parameters::setFftFrameSize(unsigned int framesize){
    if(framesize < 1) throw Exception("FFT frame size must be > 0");
    fftFrameSize = framesize;
  }
  void Parameters::setHopsPerFrame(unsigned int hpf){
    if(hpf < 1) throw Exception("Hops per frame must be > 0");
    hopsPerFrame = hpf;
  }
  void Parameters::setOctaves(unsigned int oct){
    if(oct < 1) throw Exception("Octaves must be > 0");
    octaves = oct;
    generateBinFreqs();
  }
  void Parameters::setBps(unsigned int bands){
    if(bands < 1) throw Exception("Bands per semitone must be > 0");
    bps = bands;
    generateBinFreqs();
  }
  void Parameters::setArbitrarySegments(unsigned int s){
    if(s < 1) throw Exception("Arbitrary segments must be > 0");
    arbitrarySegments = s;
  }
  void Parameters::setSegGaussianSize(unsigned int size){
    if(size < 1) throw Exception("Gaussian size must be > 0");
    segGaussianSize = size;
  }
  void Parameters::setSegGaussianSigma(float sigma){
    if(sigma <= 0) throw Exception("Gaussian sigma must be > 0");
    segGaussianSigma = sigma;
  }
  void Parameters::setStartingFreqA(float a){
    if(
      a != 27.5  && a != 55.0  && a != 110.0  && a != 220.0  &&
      a != 440.0 && a != 880.0 && a != 1760.0 && a != 3520.0
    ) throw Exception("Starting frequency must be an A (2^n * 27.5 Hz)");
    stFreq = a;
    generateBinFreqs();
  }
  void Parameters::setDirectSkStretch(float stretch){
    if(stretch <= 0) throw Exception("Spectral kernel stretch must be > 0");
    directSkStretch = stretch;
  }
  void Parameters::setDetunedBandWeight(float weight){
    if(weight < 0) throw Exception("Detuned band weighting must be >= 0");
    detunedBandWeight = weight;
  }
  void Parameters::setCustomToneProfile(const std::vector<float>& v){
    if(v.size() != 24) throw Exception("Custom tone profile must have 24 elements");
    for(unsigned int i = 0; i < 24; i++)
      if(v[i] < 0)
        throw Exception("Custom tone profile elements must be >= 0");
    // Exception handling for occasional problem on OSX Leopard.
    try{
      customToneProfile = v;
    }catch(const std::exception& e){
      throw Exception(e.what());
    }catch(...){
      throw Exception("Unknown exception setting custom tone profile");
    }
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

  float Parameters::getBinFreq(unsigned int n)const{
    unsigned int max = octaves * 12 * bps;
    if(n >= max){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds frequency index (" << n << "/" << max << ")";
      throw Exception(ss.str().c_str());
    }
    return binFreqs[n];
  }

  float Parameters::getLastFreq() const{
    return binFreqs[binFreqs.size()-1];
  }

}
