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

#include "downsampler.h"


namespace KeyFinder{

  void Downsampler::downsample(AudioData*& audioIn, const float& lastFreq, LowPassFilterFactory* lpfFactory) const{

    // TODO: there is presumably some good maths to determine filter frequencies
    float midCutoff = lastFreq * 1.05;
    float endCutoff = lastFreq * 1.10;

    unsigned int downsampleFactor = (int)floor( audioIn->getFrameRate() / 2 / endCutoff );
    if (downsampleFactor == 1) return;

    unsigned int oldFrameRate = audioIn->getFrameRate();
    unsigned int oldSampleCount = audioIn->getSampleCount();
    unsigned int channels = audioIn->getChannels();

    unsigned int newFrameRate = oldFrameRate / downsampleFactor;
    unsigned int newSampleCount = ceil((oldSampleCount / channels) / (double)downsampleFactor) * channels;

    // prep output buffer
    AudioData* audioOut = new AudioData();
    audioOut->setFrameRate(newFrameRate);
    audioOut->setChannels(channels);
    try{
      audioOut->addToSampleCount(newSampleCount);
    }catch(const Exception& e){
      delete audioOut;
      throw e;
    }

    // prep filter
    unsigned int filterOrder = 160;
    unsigned int filterDelay = filterOrder/2;
    // create circular buffer for filter delay
    Binode<float>* p = new Binode<float>(); // first node
    Binode<float>* q = p;
    for (unsigned int i=0; i<filterOrder; i++){
      q->r = new Binode<float>(); // subsequent nodes
      q->r->l = q;
      q = q->r;
    }
    // join first and last nodes
    p->l = q;
    q->r = p;

    // get filter
    LowPassFilter* lpf = lpfFactory->getLowPassFilter(filterOrder + 1, oldFrameRate, midCutoff, 2048);

    // for each channel (should be mono by this point but just in case)
    for (unsigned int ch = 0; ch < channels; ch++){
      q = p;
      // clear delay buffer
      for (unsigned int k = 0; k <= filterOrder; k++){
        q->data = 0.0;
        q = q->r;
      }
      // for each frame (running off the end of the sample stream by
      // filterDelay), get the relevant sample i for this channel
      for (int i = ch; i < (signed)(oldSampleCount + filterDelay); i += channels){

        // shuffle old samples along delay buffer
        p = p->r;

        // load new sample into delay buffer
        if (i < (signed)oldSampleCount){
          p->l->data = audioIn->getSample(i) / lpf->gain;
        }else{
          // zero pad once we're into the delay at the end of the file
          p->l->data = 0.0;
        }

        // only do the maths for the useful samples
        if ((i % (downsampleFactor * channels)) < channels){
          float sum = 0.0;
          q = p;
          for (unsigned int k = 0; k <= filterOrder; k++){
            sum += lpf->coefficients[k] * q->data;
            q = q->r;
          }
          // don't try and set samples during the warm-up, only once we've passed filterDelay samples
          if (i >= (signed)filterDelay){
            audioOut->setSample(((i-filterDelay) / downsampleFactor) + ch, sum);
          }
        }
      }
    }

    // delete delay buffer
    for (unsigned int k = 0; k <= filterOrder; k++){
      q = p;
      p = p->r;
      delete q;
    }
    // note we don't delete the LPF; it's stored in the factory for reuse
    delete audioIn;
    audioIn = audioOut;
  }

}
