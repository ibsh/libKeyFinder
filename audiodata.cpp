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

  AudioData::AudioData(): samples(0), channels(0), frameRate(0), sampleCount(0){ }

  unsigned int AudioData::getChannels() const{
    return channels;
  }

  void AudioData::setChannels(unsigned int n){
    if(n < 1) throw Exception("Channels must be > 0");
    channels = n;
  }

  unsigned int AudioData::getFrameRate() const{
    return frameRate;
  }

  void AudioData::setFrameRate(unsigned int n){
    if(n < 1) throw Exception("Frame rate must be > 0");
    frameRate = n;
  }

  // get sample by absolute index
  float AudioData::getSample(unsigned int n) const{
    if(n >= sampleCount){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << n << "/" << sampleCount << ")";
      throw Exception(ss.str().c_str());
    }
    return samples[n];
  }

  // get sample by frame and channel
  float AudioData::getSample(unsigned int frm, unsigned int ch) const{
    if(frm >= getFrameCount()){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds frame (" << frm << "/" << getFrameCount() << ")";
      throw Exception(ss.str().c_str());
    }
    if(ch >= channels){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds channel (" << ch << "/" << channels << ")";
      throw Exception(ss.str().c_str());
    }
    return getSample(frm * channels + ch);
  }

  // set sample by absolute index
  void AudioData::setSample(unsigned int n, float x){
    if(n >= sampleCount){
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << n << "/" << sampleCount << ")";
      throw Exception(ss.str().c_str());
    }
    if(!boost::math::isfinite(x)){
      throw Exception("Cannot set sample to NaN");
    }
    samples[n] = x;
  }

  // set sample by frame and channel
  void AudioData::setSample(unsigned int frm, unsigned int ch, float x){
    if(frm >= getFrameCount()){
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds frame (" << frm << "/" << getFrameCount() << ")";
      throw Exception(ss.str().c_str());
    }
    if(ch >= channels){
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds channel (" << ch << "/" << channels << ")";
      throw Exception(ss.str().c_str());
    }
    setSample(frm * channels + ch, x);
  }

  void AudioData::addToSampleCount(unsigned int newSamples){
    try{
      samples.resize(sampleCount + newSamples, 0.0);
      sampleCount += newSamples;
      // turns out this doesn't work; bad_alloc never gets thrown on Mac
      // (presumably it tries to do everything in swap).
    }catch(const std::exception& e){
      std::ostringstream ss;
      ss << "Exception adding " << newSamples << " samples to stream of " << sampleCount << ": " << e.what();
      throw Exception(ss.str().c_str());
    }catch(...){
      std::ostringstream ss;
      ss << "Unknown exception adding " << newSamples << " samples to stream of " << sampleCount;
      throw Exception(ss.str().c_str());
    }
  }

  void AudioData::addToFrameCount(unsigned int newFrames){
    if(channels < 1) throw Exception("Channels must be > 0");
    addToSampleCount(newFrames * channels);
  }

  unsigned int AudioData::getSampleCount() const{
    return sampleCount;
  }

  unsigned int AudioData::getFrameCount() const{
    if(channels < 1) throw Exception("Channels must be > 0");
    return sampleCount / channels;
  }

  void AudioData::reduceToMono(){
    if(channels == 1) return;
    std::vector<float> newStream(sampleCount / channels);
    for (unsigned int i = 0; i < sampleCount; i += channels){
      for (unsigned int j = 0; j < channels; j++){
        newStream[i/channels] += samples[i + j] / channels;
      }
    }
    samples = newStream;
    sampleCount /= channels;
    channels = 1;
    return;
  }

}
