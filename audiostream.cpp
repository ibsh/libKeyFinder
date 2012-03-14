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

#include "audiostream.h"

namespace KeyFinder{

  AudioStream::AudioStream():  stream(0), channels(0), frameRate(0),  samples(0){ }

  unsigned int AudioStream::getChannels() const{
    return channels;
  }

  void AudioStream::setChannels(unsigned int n){
    if(n < 1)
      throw Exception("Channels must be > 0");
    channels = n;
  }

  unsigned int AudioStream::getFrameRate() const{
    return frameRate;
  }

  void AudioStream::setFrameRate(unsigned int n){
    if(n < 1)
      throw Exception("Frame rate must be > 0");
    frameRate = n;
  }

  float AudioStream::getSample(unsigned int n) const{
    if(n >= samples){
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << n << "/" << samples << ")";
      throw Exception(ss.str());
    }
    return stream[n];
  }

  void AudioStream::setSample(unsigned int n,float x){
    if(n >= samples){
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << n << "/" << samples << ")";
      throw Exception(ss.str());
    }
    stream[n] = x;
  }

  void AudioStream::addToSampleCount(unsigned int newSamples){
    try{
      stream.resize(samples + newSamples);
      samples += newSamples;
    }catch(...){
      std::ostringstream ss;
      ss << "Memory allocation failure adding " << newSamples << " samples to audio stream of size " << samples;
      throw Exception(ss.str());
    }
  }

  unsigned int AudioStream::getSampleCount() const{
    return samples;
  }

  void AudioStream::reduceToMono(){
    if(channels == 1) return;
    std::vector<float> newStream(samples / channels);
    for (unsigned int i = 0; i < samples; i += channels){
      float mono = 0.0;
      for (unsigned int j = 0; j < channels; j++)
        mono += stream[i + j];
      mono /= channels;
      newStream[i/channels] = mono;
    }
    stream = newStream;
    samples /= channels;
    channels = 1;
    return;
  }

} // namespace
