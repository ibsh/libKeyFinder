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

#include "audiodata.h"

namespace KeyFinder{

  AudioData::AudioData():  samples(0), channels(0), frameRate(0),  sampleCount(0){ }

  unsigned int AudioData::getChannels() const{
    return channels;
  }

  void AudioData::setChannels(unsigned int n){
    if(n < 1)
      throw Exception("Channels must be > 0");
    channels = n;
  }

  unsigned int AudioData::getFrameRate() const{
    return frameRate;
  }

  void AudioData::setFrameRate(unsigned int n){
    if(n < 1)
      throw Exception("Frame rate must be > 0");
    frameRate = n;
  }

  float AudioData::getSample(unsigned int n) const{
    if(n >= sampleCount){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << n << "/" << sampleCount << ")";
      throw Exception(ss.str());
    }
    return samples[n];
  }

  void AudioData::setSample(unsigned int n, float x){
    if(n >= sampleCount){
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << n << "/" << sampleCount << ")";
      throw Exception(ss.str());
    }
    samples[n] = x;
  }

  void AudioData::addToSampleCount(unsigned int newSamples){
    try{
      samples.resize(sampleCount + newSamples);
      sampleCount += newSamples;
    }catch(...){
      std::ostringstream ss;
      ss << "Memory allocation failure adding " << newSamples << " samples to audio stream of size " << sampleCount;
      throw Exception(ss.str());
    }
  }

  unsigned int AudioData::getSampleCount() const{
    return sampleCount;
  }

  void AudioData::reduceToMono(){
    if(channels == 1) return;
    std::vector<float> newStream(sampleCount / channels);
    for (unsigned int i = 0; i < sampleCount; i += channels)
      for (unsigned int j = 0; j < channels; j++)
        newStream[i/channels] += samples[i + j] / channels;
    samples = newStream;
    sampleCount /= channels;
    channels = 1;
    return;
  }

}
