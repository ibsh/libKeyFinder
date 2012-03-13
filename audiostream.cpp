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
    channels = n;
  }

  unsigned int AudioStream::getFrameRate() const{
    return frameRate;
  }

  void AudioStream::setFrameRate(unsigned int n){
    frameRate = n;
  }

  float AudioStream::getSample(unsigned int n) const{
    if(n < samples){
      return stream[n];
    }else{
      qDebug("Attempt to get out-of-bounds sample (%d/%d)",n,samples);
      return 0;
    }
  }

  void AudioStream::setSample(unsigned int n,float x){
    if(n < samples)
      stream[n] = x;
    else
      qDebug("Attempt to set out-of-bounds sample (%d/%d)",n,samples);
  }

  void AudioStream::addToSampleCount(unsigned int newSamples){
    try{
      stream.resize(samples + newSamples);
      samples += newSamples;
    }catch(...){
      qCritical("Memory allocation failure adding %d samples to audio stream of size %d", newSamples, samples);
      throw Exception();
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
