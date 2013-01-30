#include "binodetest.h"

TEST(BinodeTest, ConstructorDefaultsWork){
  KeyFinder::Binode<float> bf;
  EXPECT_EQ(0.0, bf.data);
  EXPECT_EQ(NULL, bf.l);
  EXPECT_EQ(NULL, bf.r);

  KeyFinder::Binode<int> bi;
  EXPECT_EQ(0, bi.data);
  EXPECT_EQ(NULL, bi.l);
  EXPECT_EQ(NULL, bi.r);
}

TEST(BinodeTest, ConstructorArgumentsWork){
  KeyFinder::Binode<float> bf(365.25);
  EXPECT_EQ(365.25, bf.data);
  EXPECT_EQ(NULL, bf.l);
  EXPECT_EQ(NULL, bf.r);

  KeyFinder::Binode<int> bi(14);
  EXPECT_EQ(14, bi.data);
}
