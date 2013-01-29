#include "audiodatatest.h"

TEST_F(AudioDataTest, ConstructorWorks) {
  KeyFinder::AudioData a;
  EXPECT_EQ(0, a.getChannels());
  EXPECT_EQ(0, a.getFrameRate());
  EXPECT_EQ(0, a.getSampleCount());
}
