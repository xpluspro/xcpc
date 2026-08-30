#include <gtest/gtest.h>
#include "prelude/common.hpp"

static constexpr u128 P = 1000000007;

#include "src/sections/String/assets/String/hash.cpp"


TEST(HashCheck, ModularEquality) {
  std::mt19937_64 rng(9);
  for (int t = 0; t < 300; ++t) {
    i128 a = (i128)(rng() % 1000000000000ULL);
    i128 b = a + (i128)P * (i128)(rng() % 20);
    EXPECT_TRUE(check(a, b));
    if (t % 3 == 0) EXPECT_TRUE(check(b, a));
    i128 c = a + 1;
    if ((c - a) % (i128)P != 0) EXPECT_FALSE(check(a, c));
  }
  EXPECT_TRUE(check(0, 0));
  EXPECT_TRUE(check((i128)P, 0));
  EXPECT_FALSE(check(1, 0));
}
