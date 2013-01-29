#include "chromagramtest.h"

TEST_F(ChromagramTest, ConstructorDefaultsWork) {
  KeyFinder::Chromagram c;
  EXPECT_EQ(0, c.getBins());
  EXPECT_EQ(0, c.getHops());
}
