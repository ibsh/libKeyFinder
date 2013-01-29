#include "chromagramtest.h"

TEST(ChromagramTest, ConstructorDefaultsWork){
  KeyFinder::Chromagram c;
  EXPECT_EQ(0, c.getHops());
  EXPECT_EQ(0, c.getBins());
}

TEST(ChromagramTest, ConstructorArgumentsWork){
  KeyFinder::Chromagram c(20, 12);
  EXPECT_EQ(20, c.getHops());
  EXPECT_EQ(12, c.getBins());
  // init values
  for(int h=0; h<20; h++){
    for(int b=0; b<12; b++){
      EXPECT_EQ(0.0, c.getMagnitude(h, b));
    }
  }
}

TEST(ChromagramTest, Mutator){
  KeyFinder::Chromagram c(1, 1);
  c.setMagnitude(0,0, 100.0);
  EXPECT_EQ(100.0, c.getMagnitude(0, 0));
}

TEST(ChromagramTest, Bounds){
  KeyFinder::Chromagram c(5, 12);
  // hops min max
  EXPECT_THROW(c.getMagnitude(-1,  0), KeyFinder::Exception);
  EXPECT_THROW(c.getMagnitude( 5,  0), KeyFinder::Exception);
  EXPECT_THROW(c.setMagnitude(-1,  0, 1.0), KeyFinder::Exception);
  EXPECT_THROW(c.setMagnitude( 5,  0, 1.0), KeyFinder::Exception);
  // bins min max
  EXPECT_THROW(c.getMagnitude( 0, -1), KeyFinder::Exception);
  EXPECT_THROW(c.getMagnitude( 0, 12), KeyFinder::Exception);
  EXPECT_THROW(c.setMagnitude( 0, -1, 1.0), KeyFinder::Exception);
  EXPECT_THROW(c.setMagnitude( 0, 12, 1.0), KeyFinder::Exception);
  // value bounds
  EXPECT_THROW(c.setMagnitude( 0,  0, INFINITY), KeyFinder::Exception);
  EXPECT_THROW(c.setMagnitude( 0,  0, NAN), KeyFinder::Exception);
}

TEST(ChromagramTest, NilTuning){
  KeyFinder::Chromagram c(1, 12);
  KeyFinder::Parameters p;
  p.setOctaves(1);

  p.setTuningMethod(KeyFinder::TUNING_BIN_ADAPTIVE);
  c.reduceTuningBins(p);
  EXPECT_EQ(12, c.getBins());

  p.setTuningMethod(KeyFinder::TUNING_HARTE);
  c.reduceTuningBins(p);
  EXPECT_EQ(12, c.getBins());
}

TEST(ChromagramTest, AdaptiveTuning){
  FAIL();
}

TEST(ChromagramTest, HarteTuning){
  FAIL();
}
