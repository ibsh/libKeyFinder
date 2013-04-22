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

#include "chromagram.h"

namespace KeyFinder {

  Chromagram::Chromagram(unsigned int hops, unsigned int oct, unsigned int bps) :
    bandsPerSemitone(bps), octaves(oct),
    chromaData(hops, std::vector<float>(bps * oct * SEMITONES, 0.0))
  { }

  float Chromagram::getMagnitude(unsigned int h, unsigned int b) const {
    if (h >= getHops()) {
      std::ostringstream ss;
      ss << "Cannot get magnitude of out-of-bounds hop (" << h << "/" << getHops() << ")";
      throw Exception(ss.str().c_str());
    }
    if (b >= getBands()) {
      std::ostringstream ss;
      ss << "Cannot get magnitude of out-of-bounds band (" << b << "/" << getBands() << ")";
      throw Exception(ss.str().c_str());
    }
    return chromaData[h][b];
  }

  void Chromagram::setMagnitude(unsigned int h, unsigned int b, float val) {
    if (h >= getHops()) {
      std::ostringstream ss;
      ss << "Cannot set magnitude of out-of-bounds hop (" << h << "/" << getHops() << ")";
      throw Exception(ss.str().c_str());
    }
    if (b >= getBands()) {
      std::ostringstream ss;
      ss << "Cannot set magnitude of out-of-bounds band (" << b << "/" << getBands() << ")";
      throw Exception(ss.str().c_str());
    }
    if (!boost::math::isfinite(val)) {
      throw Exception("Cannot set magnitude to NaN");
    }
    chromaData[h][b] = val;
  }

  void Chromagram::tuningHarte() {
    /*
     * This is quite involved, and it's only an approximation of Harte's method
     * based on his thesis rather than a port of his code, but it works well for
     * e.g. Strawberry Fields Forever and other recordings he mentioned as being
     * difficult from a tuning perspective.
     */
    if (bandsPerSemitone == 1) return;
    // find peaks; anything that's higher energy than the mean for this hop and higher energy than its neighbours.
    std::vector< std::vector<float> > peakLocations;
    std::vector< std::vector<float> > peakMagnitudes;
    for (unsigned int hop = 0; hop < getHops(); hop++) {
      // find mean magnitude for this hop
      float meanVal = 0.0;
      for (unsigned int band = 0; band < getBands(); band++)
        meanVal += chromaData[hop][band] / getBands();
      // find peak bands
      std::vector<unsigned int> peakBands;
      for (unsigned int band = 0; band < getBands(); band++) {
        float bandVal = getMagnitude(hop, band);
        if (bandVal > meanVal)
          if (band == 0 || bandVal > getMagnitude(hop, band - 1))
            if (band == getBands() -1 || bandVal > getMagnitude(hop, band + 1))
              peakBands.push_back(band);
      }
      // quadratic interpolation to find a more precise peak position and magnitude.
      std::vector<float> peakLocationsForThisHop;
      std::vector<float> peakMagnitudesForThisHop;
      for (unsigned int peak = 0; peak < peakBands.size(); peak++) {
        // substitute 0 for neighbouring band if peak is at an extremity of the vector
        float alpha = peakBands[peak] == 0 ? 0 : getMagnitude(hop, peakBands[peak] - 1);
        float beta  = getMagnitude(hop, peakBands[peak]);
        float gamma = peakBands[peak] == getBands() - 1 ? 0 : getMagnitude(hop, peakBands[peak] + 1);
        float peakLocation = ((alpha -  gamma) / (alpha - (2 * beta) + gamma)) / 2;
        float peakMagnitude = beta - ((1/4) * (alpha - gamma) * peakLocation);
        peakLocationsForThisHop.push_back(peakBands[peak] + peakLocation);
        peakMagnitudesForThisHop.push_back(peakMagnitude);
      }
      peakLocations.push_back(peakLocationsForThisHop);
      peakMagnitudes.push_back(peakMagnitudesForThisHop);
    }
    // determine tuning distribution of peaks across all hops. Centre band = concert tuning.
    std::vector<float> peakTuningDistribution(bandsPerSemitone * 10, 0.0);
    for (unsigned int hop = 0; hop < getHops(); hop++) {
      for (unsigned int peak = 0; peak < peakLocations[hop].size(); peak++) {
        float peakLocationMod = fmodf(peakLocations[hop][peak], (float)bandsPerSemitone) * 10.0;
        unsigned int peakLocationInt = (int)(peakLocationMod + 0.5) + 5;
        peakTuningDistribution[peakLocationInt % (bandsPerSemitone * 10)] += peakMagnitudes[hop][peak];
      }
    }
    // now find the tuning peak; the tenth of a semitone that the most peaks are tuned to.
    float tuningMax = 0;
    unsigned int tuningPeak = -1;
    for (unsigned int i = 0; i < bandsPerSemitone * 10; i++) {
      if (peakTuningDistribution[i] > tuningMax) {
        tuningMax = peakTuningDistribution[i];
        tuningPeak = i;
      }
    }
    // now discard (zero out, for ease) any peaks that sit >= 0.2 semitones (e.g. 6 bands for 3bps) away from the tuning peak.
    // figure out which tuning bands to keep
    std::vector<unsigned int> bandsToKeep;
    for (int i = (1 - (bandsPerSemitone * 2)); i < (signed)bandsPerSemitone * 2; i++)
      bandsToKeep.push_back((tuningPeak + i + (bandsPerSemitone*10)) % (bandsPerSemitone*10));
    // and discard the others
    std::vector< std::vector<float> > twelveBpoChroma(getHops(), std::vector<float>(SEMITONES * octaves));
    for (unsigned int hop = 0; hop < getHops(); hop++) {
      for (unsigned int peak = 0; peak < peakLocations[hop].size(); peak++) {
        float peakLocationMod = fmodf(peakLocations[hop][peak], (float)bandsPerSemitone);
        peakLocationMod *= 10;
        unsigned int peakLocationInt = peakLocationMod + 0.5;
        peakLocationInt += 5;
        bool discardMe = true;
        for (unsigned int i=0; i<bandsToKeep.size(); i++)
          if (peakLocationInt == bandsToKeep[i])
            discardMe = false;
        if (!discardMe) { // this is a valid peak for the tuned chromagram
          unsigned int tunedPeakLocation = (unsigned int)peakLocations[hop][peak];
          tunedPeakLocation /= bandsPerSemitone;
          twelveBpoChroma[hop][tunedPeakLocation] += peakMagnitudes[hop][peak];
        }
      }
    }
    chromaData = twelveBpoChroma;
    bandsPerSemitone = 1;
  }

  void Chromagram::tuningBandAdaptive(float detunedBandWeight) {
    /*
     * This is designed to tune for each semitone band rather than for the whole
     * recording; aimed at dance music with individually detuned elements, rather
     * than music that is internally consistent but off concert pitch.
     */
    if (bandsPerSemitone == 1) return;
    std::vector< std::vector<float> > twelveBpoChroma(getHops(), std::vector<float>(SEMITONES * octaves));
    for (unsigned int st = 0; st < SEMITONES * octaves; st++) {
      std::vector<float> oneSemitoneChroma(bandsPerSemitone);
      for (unsigned int h = 0; h < getHops(); h++)
        for (unsigned int b = 0; b < bandsPerSemitone; b++)
          oneSemitoneChroma[b] += chromaData[h][st*bandsPerSemitone+b];
      // determine highest energy tuning band
      unsigned int whichband = 0;
      float max = oneSemitoneChroma[0];
      for (unsigned int i=1; i<bandsPerSemitone; i++) {
        if (oneSemitoneChroma[i] > max) {
          max = oneSemitoneChroma[i];
          whichband = i;
        }
      }
      for (unsigned int h = 0; h < getHops(); h++) {
        float weighted = 0.0;
        for (unsigned int b = 0; b < bandsPerSemitone; b++)
          weighted += (chromaData[h][st*bandsPerSemitone+b] * (b == whichband ? 1.0 : detunedBandWeight));
        twelveBpoChroma[h][st] = weighted;
      }
    }
    chromaData = twelveBpoChroma;
    bandsPerSemitone = 1;
  }

  void Chromagram::reduceToOneOctave () {
    if (octaves == 1)
      return;
    unsigned int bandsPerOctave = bandsPerSemitone * SEMITONES;
    std::vector< std::vector<float> > oneOctaveChroma(getHops(), std::vector<float>(bandsPerOctave));
    for (unsigned int h = 0; h < getHops(); h++) {
      for (unsigned int b = 0; b < bandsPerOctave; b++) {
        float sum = 0.0;
        for (unsigned int o = 0; o < octaves; o++)
          sum += chromaData[h][o * bandsPerOctave + b];
        oneOctaveChroma[h][b] = sum / octaves;
      }
    }
    chromaData = oneOctaveChroma;
    octaves = 1;
  }

  void Chromagram::append(const Chromagram& that) {
    if (octaves == 0 && bandsPerSemitone == 0) {
      octaves = that.octaves;
      bandsPerSemitone = that.bandsPerSemitone;
    }
    if (that.octaves != octaves || that.bandsPerSemitone != bandsPerSemitone)
      throw Exception("Cannot append a chromagram with a different number of bands");
    unsigned int oldHops = getHops();
    chromaData.resize(getHops() + that.getHops(), std::vector<float>(getBands(), 0.0));
    for (unsigned int h = 0; h < that.getHops(); h++)
      for (unsigned int b = 0; b < that.getBands(); b++)
        setMagnitude(oldHops + h, b, that.getMagnitude(h, b));
  }

  unsigned int Chromagram::getHops() const {
    return chromaData.size();
  }

  unsigned int Chromagram::getBands() const {
    return bandsPerSemitone * octaves * SEMITONES;
  }

  unsigned int Chromagram::getBandsPerSemitone() const {
    return bandsPerSemitone;
  }

  unsigned int Chromagram::getOctaves() const {
    return octaves;
  }

}
