#include <gtest/gtest.h>
#include "myint.hpp"

namespace {

class MyIntTest : public ::testing::Test{};

TEST_F(MyIntTest, isOdd)
{
  MyInt mi1 = MyInt(10);
  EXPECT_EQ(false, mi1.isOdd());

  MyInt mi2 = MyInt(13);
  EXPECT_EQ(true, mi2.isOdd());
}


TEST_F(MyIntTest, isEven)
{
  MyInt mi1 = MyInt(10);
  EXPECT_EQ(true, mi1.isEven());

  MyInt mi2 = MyInt(13);
  EXPECT_EQ(false, mi2.isEven());
}

} // namespace