#include "audiodatatest.h"

TEST(AudioDataTest, ConstructorWorks){
  KeyFinder::AudioData a;
  EXPECT_EQ(0, a.getChannels());
  EXPECT_EQ(0, a.getFrameRate());
  EXPECT_EQ(0, a.getSampleCount());
}

TEST(AudioDataTest, Channels){
  KeyFinder::AudioData a;
  a.setChannels(2);
  EXPECT_EQ(2, a.getChannels());
  EXPECT_THROW(a.setChannels(0), KeyFinder::Exception);
}

TEST(AudioDataTest, FrameRate){
  KeyFinder::AudioData a;
  a.setFrameRate(44100);
  EXPECT_EQ(44100, a.getFrameRate());
  EXPECT_THROW(a.setFrameRate(0), KeyFinder::Exception);
}

TEST(AudioDataTest, SamplesBasic){
  KeyFinder::AudioData a;
  a.addToSampleCount(100);
  EXPECT_EQ(100, a.getSampleCount());
  // init values
  for(int i=0; i<100; i++){
    EXPECT_EQ(0.0, a.getSample(i));
  }
  a.setSample(0, 10.0);
  EXPECT_EQ(10.0, a.getSample(0));
}

TEST(AudioDataTest, Bounds){
  KeyFinder::AudioData a;
  a.addToSampleCount(5);
  EXPECT_THROW(a.getSample(-1), KeyFinder::Exception);
  EXPECT_THROW(a.getSample(5), KeyFinder::Exception);
  EXPECT_THROW(a.setSample(-1, 1.0), KeyFinder::Exception);
  EXPECT_THROW(a.setSample(5, 1.0), KeyFinder::Exception);
  EXPECT_THROW(a.setSample(0, INFINITY), KeyFinder::Exception);
  EXPECT_THROW(a.setSample(0, NAN), KeyFinder::Exception);
}

TEST(AudioDataTest, MakeMono){
  KeyFinder::AudioData a;
  a.setChannels(2);
  a.addToSampleCount(20);
  for(int i=0; i<10; i++){
    a.setSample(i*2, 100.0);
  }
  a.reduceToMono();
  EXPECT_EQ(10, a.getSampleCount());
  for(int i=0; i<10; i++){
    EXPECT_EQ(50.0, a.getSample(i));
  }
}

//TEST(AudioDataTest, SamplesHeavy) {
//  KeyFinder::AudioData a;
//  EXPECT_THROW(a.addToSampleCount(UINT_MAX), KeyFinder::Exception);
//}
