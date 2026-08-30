#include <gtest/gtest.h>
#define N 256
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/Pell方程.cpp"


static bool is_square(LL n) {
  LL s = (LL)llround(sqrt((long double)n));
  return s * s == n;
}

TEST(Pell, KnownMinimal) {
  EXPECT_EQ(peLL(2), (pair<LL, LL>{3, 2}));
  EXPECT_EQ(peLL(3), (pair<LL, LL>{2, 1}));
  EXPECT_EQ(peLL(13), (pair<LL, LL>{649, 180}));
}

TEST(Pell, SquareHasNoSolution) {
  EXPECT_EQ(peLL(9), (pair<LL, LL>{0, 0}));
  EXPECT_EQ(peLL(4), (pair<LL, LL>{0, 0}));
}

TEST(Pell, IdentityAndMinimality) {
  for (LL n = 2; n <= 40; ++n) {
    if (is_square(n)) continue;
    auto [x, y] = peLL(n);
    EXPECT_GT(x, 0);
    EXPECT_GT(y, 0);
    EXPECT_EQ((__int128)x * x - (__int128)n * y * y, 1);
    bool smaller = false;
    for (LL xx = 1; xx < x; ++xx) {
      __int128 need = (__int128)xx * xx - 1;
      if (need % n == 0) {
        __int128 yy2 = need / n;
        LL yy = (LL)llround(sqrt((long double)yy2));
        if ((__int128)yy * yy == yy2 && yy > 0) smaller = true;
      }
    }
    EXPECT_FALSE(smaller) << n;
  }
}
