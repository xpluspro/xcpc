#include <gtest/gtest.h>
#define N 64
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/直线下格点统计.cpp"


static LL brute_floor_sum(LL n, LL a, LL b, LL m) {
  LL s = 0;
  for (LL i = 0; i < n; ++i) s += (a + b * i) / m;
  return s;
}

TEST(FloorSum, MatchesBrute) {
  EXPECT_EQ(solve(5, 0, 0, 3), brute_floor_sum(5, 0, 0, 3));
  EXPECT_EQ(solve(1, 0, 0, 1), 0);
  EXPECT_EQ(solve(10, 7, 0, 3), brute_floor_sum(10, 7, 0, 3));
  EXPECT_EQ(solve(10, 1, 5, 3), brute_floor_sum(10, 1, 5, 3));
  EXPECT_EQ(solve(8, 20, 3, 7), brute_floor_sum(8, 20, 3, 7));
  std::mt19937_64 rng(5);
  for (int t = 0; t < 300; ++t) {
    LL n = (LL)(rng() % 80 + 1);
    LL m = (LL)(rng() % 80 + 1);
    LL a = (LL)(rng() % 120);
    LL b = (LL)(rng() % 120);
    EXPECT_EQ(solve(n, a, b, m), brute_floor_sum(n, a, b, m))
        << n << " " << a << " " << b << " " << m;
  }
}
