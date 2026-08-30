#include <gtest/gtest.h>
#define LOG 30
#define MOD 998244353
#include "prelude/common.hpp"
#include "src/sections/Polynomial/assets/Math/线性递推.cpp"


TEST(LinearRec, CommentExample) {
  // Printed example: first = {a_1, a_2} = {1, 3}, trans = {2, 1}
  // (a_n = 2 a_{n-1} + a_{n-2}), calc(k) returns a_k. a_0 is not stored.
  poly first = {1, 3};
  poly trans = {2, 1};
  LinearRec lr(first, trans);
  EXPECT_EQ(lr.calc(1), 1);
  EXPECT_EQ(lr.calc(2), 3);
  EXPECT_EQ(lr.calc(3), 7);
  EXPECT_EQ(lr.calc(4), 17);
  EXPECT_EQ(lr.calc(5), 41);
}
