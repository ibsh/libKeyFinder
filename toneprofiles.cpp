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

#include "toneprofiles.h"

namespace KeyFinder{

  ToneProfile::ToneProfile(tone_profile_t whichProfile, scale_t scale, const Parameters& params){

    float p[12];
    if(whichProfile == TONE_PROFILE_SILENCE){
      p[0]=0; p[1]=0;
      p[2]=0; p[3]=0;
      p[4]=0; p[5]=0;
      p[6]=0; p[7]=0;
      p[8]=0; p[9]=0;
      p[10]=0; p[11]=0;
    }else if(whichProfile == TONE_PROFILE_TEMPERLEY){
      if(scale == SCALE_MAJOR){
        p[0]=5.0; p[1]=2.0;
        p[2]=3.5; p[3]=2.0;
        p[4]=4.5;
        p[5]=4.0; p[6]=2.0;
        p[7]=4.5; p[8]=2.0;
        p[9]=3.5; p[10]=1.5;
        p[11]=4.0;
      }else{
        p[0]=5.0; p[1]=2.0;
        p[2]=3.5;
        p[3]=4.5; p[4]=2.0;
        p[5]=4.0; p[6]=2.0;
        p[7]=4.5;
        p[8]=3.5; p[9]=2.0;
        p[10]=1.5; p[11]=4.0;
      }
    }else if(whichProfile == TONE_PROFILE_GOMEZ){
      if(scale == SCALE_MAJOR){
        p[0]=0.82; p[1]=0.00;
        p[2]=0.55; p[3]=0.00;
        p[4]=0.53;
        p[5]=0.30; p[6]=0.08;
        p[7]=1.00; p[8]=0.00;
        p[9]=0.38; p[10]=0.00;
        p[11]=0.47;
      }else{
        p[0]=0.81; p[1]=0.00;
        p[2]=0.53;
        p[3]=0.54; p[4]=0.00;
        p[5]=0.27; p[6]=0.07;
        p[7]=1.00;
        p[8]=0.27; p[9]=0.07;
        p[10]=0.10; p[11]=0.36;
      }
    }else if(whichProfile == TONE_PROFILE_SHAATH){
      if(scale == SCALE_MAJOR){
        p[0]=6.6; p[1]=2.0;
        p[2]=3.5; p[3]=2.3;
        p[4]=4.6;
        p[5]=4.0; p[6]=2.5;
        p[7]=5.2; p[8]=2.4;
        p[9]=3.7; p[10]=2.3;
        p[11]=3.4;
      }else{
        p[0]=6.5; p[1]=2.7;
        p[2]=3.5;
        p[3]=5.4; p[4]=2.6;
        p[5]=3.5; p[6]=2.5;
        p[7]=5.2;
        p[8]=4.0; p[9]=2.7;
        p[10]=4.3; p[11]=3.2;
      }
    }else if(whichProfile == TONE_PROFILE_KRUMHANSL){
      if(scale == SCALE_MAJOR){
        p[0]=6.35; p[1]=2.23;
        p[2]=3.48; p[3]=2.33;
        p[4]=4.38;
        p[5]=4.09; p[6]=2.52;
        p[7]=5.19; p[8]=2.39;
        p[9]=3.66; p[10]=2.29;
        p[11]=2.88;
      }else{
        p[0]=6.33; p[1]=2.68;
        p[2]=3.52;
        p[3]=5.38; p[4]=2.60;
        p[5]=3.53; p[6]=2.54;
        p[7]=4.75;
        p[8]=3.98; p[9]=2.69;
        p[10]=3.34; p[11]=3.17;
      }
    }else{ // Custom
      std::vector<float> ctp = params.getCustomToneProfile();
      if(scale == SCALE_MAJOR){
        for (unsigned int i=0; i<12; i++)
          p[i] = (float)ctp[i];
      }else{
        for (unsigned int i=0; i<12; i++)
          p[i] = (float)ctp[i+12];
      }
    }

    // copy into doubly-linked circular list
    tonic = new Binode<float>(p[0]);
    Binode<float> *q = tonic;
    for (unsigned int i=1; i<12; i++){
      q->r = new Binode<float>(p[i]);
      q->r->l = q;
      q = q->r;
    }
    q->r = tonic;
    tonic->l = q;
    // offset from A to C (3 semitones) if specified in Parameters
    if(params.getOffsetToC()){
      for (unsigned int i=0; i<3; i++)
        tonic = tonic->r;
    }

    // get mean in preparation for correlation
    profileMean = 0.0;
    for (unsigned int i=0; i<12; i++)
      profileMean += (p[i] / 12.0);
  }

  ToneProfile::~ToneProfile(){
    free();
  }

  void ToneProfile::free(){
    Binode<float>* p = tonic;
    do{
      Binode<float>* zap = p;
      p = p->r;
      delete zap;
    }while(p!=tonic);
  }

  float ToneProfile::similarity(similarity_measure_t measure, const std::vector<float>& input, int offset, float inputMean){
    if(measure == SIMILARITY_CORRELATION)
      return correlation(input, offset, inputMean);
    else
      return cosine(input, offset);
  }

  /*
  Determines cosine similarity between input vector and profile scale.
  input = array of 12 floats relating to an octave starting at A natural
  offset = which scale to test against; 0 = A, 1 = Bb, 2 = B, 3 = C etc
  */
  float ToneProfile::cosine(const std::vector<float>& input, int offset) const{
    // Rotate starting pointer left for offset. Each step shifts the position
    // of the tonic one step further right of the starting pointer (or one semitone up).
    Binode<float>* p = tonic;
    for (int i=0; i<offset; i++)
      p = p->l;
    float intersection = 0.0;
    float profileNorm = 0.0;
    float inputNorm = 0.0;
    for (int i=0; i<12; i++){
      intersection += input[i] * p->data;
      profileNorm += pow((p->data),2);
      inputNorm += pow((input[i]),2);
      p = p->r;
    }
    if(profileNorm > 0 && inputNorm > 0) // divzero
      return intersection / (sqrt(profileNorm) * sqrt(inputNorm));
    else
      return 0;
  }

  /*
  Krumhansl's correlation between input vector and profile scale.
  input = array of 12 floats relating to an octave starting at A natural
  offset = which scale to test against; 0 = A, 1 = Bb, 2 = B, 3 = C etc
  */
  float ToneProfile::correlation(const std::vector<float>& input, int offset, float inputMean) const{
    Binode<float>* p = tonic;
    for (int i=0; i<offset; i++)
      p = p->l;
    float sumTop = 0.0;
    float sumBottomLeft = 0.0;
    float sumBottomRight = 0.0;
    for (int i=0; i<12; i++){
      float xMinusXBar = p->data - profileMean;
      float yMinusYBar = input[i] - inputMean;
      sumTop += xMinusXBar * yMinusYBar;
      sumBottomLeft += pow(xMinusXBar,2);
      sumBottomRight += pow(yMinusYBar,2);
      p = p->r;
    }
    if(sumBottomRight > 0 && sumBottomLeft > 0) // divzero
      return sumTop / sqrt(sumBottomLeft * sumBottomRight);
    else
      return 0;
  }

}
