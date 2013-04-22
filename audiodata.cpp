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

namespace KeyFinder {

  AudioData::AudioData(): samples(0), channels(0), frameRate(0) { }

  unsigned int AudioData::getChannels() const {
    return channels;
  }

  void AudioData::setChannels(unsigned int newChannels) {
    if (newChannels < 1) throw Exception("Channels must be > 0");
    channels = newChannels;
  }

  unsigned int AudioData::getFrameRate() const {
    return frameRate;
  }

  void AudioData::setFrameRate(unsigned int newFrameRate) {
    if (newFrameRate < 1) throw Exception("Frame rate must be > 0");
    frameRate = newFrameRate;
  }

  void AudioData::append(const AudioData& that) {
    if (channels == 0 && frameRate == 0) {
      channels = that.channels;
      frameRate = that.frameRate;
    }
    if (that.channels != channels)
      throw Exception("Cannot append audio data with a different number of channels");
    if (that.frameRate != frameRate)
      throw Exception("Cannot append audio data with a different frame rate");
    unsigned int oldSampleCount = getSampleCount();
    samples.resize(oldSampleCount + that.getSampleCount(), 0.0);
    for (unsigned int s = 0; s < that.getSampleCount(); s++)
      setSample(oldSampleCount + s, that.getSample(s));
  }

  // get sample by absolute index
  float AudioData::getSample(unsigned int index) const {
    if (index >= getSampleCount()) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds sample (" << index << "/" << getSampleCount() << ")";
      throw Exception(ss.str().c_str());
    }
    return samples[index];
  }

  // get sample by frame and channel
  float AudioData::getSample(unsigned int frame, unsigned int channel) const {
    if (frame >= getFrameCount()) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds frame (" << frame << "/" << getFrameCount() << ")";
      throw Exception(ss.str().c_str());
    }
    if (channel >= channels) {
      std::ostringstream ss;
      ss << "Cannot get out-of-bounds channel (" << channel << "/" << channels << ")";
      throw Exception(ss.str().c_str());
    }
    return getSample(frame * channels + channel);
  }

  // set sample by absolute index
  void AudioData::setSample(unsigned int index, float value) {
    if (index >= getSampleCount()) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds sample (" << index << "/" << getSampleCount() << ")";
      throw Exception(ss.str().c_str());
    }
    if (!boost::math::isfinite(value)) {
      throw Exception("Cannot set sample to NaN");
    }
    samples[index] = value;
  }

  // set sample by frame and channel
  void AudioData::setSample(unsigned int frame, unsigned int channel, float value) {
    if (frame >= getFrameCount()) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds frame (" << frame << "/" << getFrameCount() << ")";
      throw Exception(ss.str().c_str());
    }
    if (channel >= channels) {
      std::ostringstream ss;
      ss << "Cannot set out-of-bounds channel (" << channel << "/" << channels << ")";
      throw Exception(ss.str().c_str());
    }
    setSample(frame * channels + channel, value);
  }

  void AudioData::addToSampleCount(unsigned int newSamples) {
    try{
      samples.resize(getSampleCount() + newSamples, 0.0);
      // TODO: turns out this doesn't work; bad_alloc never gets thrown on Mac,
      // presumably it tries to do everything in swap
    }catch(const std::exception& e) {
      std::ostringstream ss;
      ss << "Exception adding " << newSamples << " samples to stream of " << getSampleCount() << ": " << e.what();
      throw Exception(ss.str().c_str());
    }catch(...) {
      std::ostringstream ss;
      ss << "Unknown exception adding " << newSamples << " samples to stream of " << getSampleCount();
      throw Exception(ss.str().c_str());
    }
  }

  void AudioData::addToFrameCount(unsigned int newFrames) {
    if (channels < 1) throw Exception("Channels must be > 0");
    addToSampleCount(newFrames * channels);
  }

  unsigned int AudioData::getSampleCount() const {
    return samples.size();
  }

  unsigned int AudioData::getFrameCount() const {
    if (channels < 1) throw Exception("Channels must be > 0");
    return getSampleCount() / channels;
  }

  void AudioData::reduceToMono() {
    if (channels == 1) return;
    std::vector<float> newStream(getSampleCount() / channels);
    for (unsigned int i = 0; i < getSampleCount(); i += channels) {
      for (unsigned int j = 0; j < channels; j++) {
        newStream[i/channels] += samples[i + j] / channels;
      }
    }
    samples = newStream;
    channels = 1;
    return;
  }

  void AudioData::discardFramesFromFront(unsigned int discardFrameCount) {
    if (discardFrameCount > getFrameCount()) {
      std::ostringstream ss;
      ss << "Cannot discard " << discardFrameCount << " frames of " << getFrameCount();
      throw Exception(ss.str().c_str());
    }
    unsigned int discardSampleCount = discardFrameCount * channels;
    unsigned int newSampleCount = getSampleCount() - discardSampleCount;
    std::vector<float> newStream(newSampleCount);
    for (unsigned int i = 0; i < newSampleCount; i ++) {
      newStream[i] = samples[i + discardSampleCount];
    }
    samples = newStream;
  }

}
