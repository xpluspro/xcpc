#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/exgcd.cpp"
#include "src/sections/NumberTheory/assets/Math/CRT_lbn.cpp"

TEST(Exgcd, BezoutRandom) {
  std::mt19937_64 rng(1);
  for (int t = 0; t < 400; ++t) {
    LL a = (LL)rng() % 2000000 - 1000000;
    LL b = (LL)rng() % 2000000 - 1000000;
    if (t == 0) a = 0, b = 0;
    if (t == 1) a = 0, b = 15;
    if (t == 2) a = 15, b = 0;
    if (t == 3) a = 1, b = 1;
    if (t == 4) a = -12, b = 18;
    LL x, y;
    LL g = exgcd(a, b, x, y);
    EXPECT_EQ(a * x + b * y, g);
    if (a == 0 && b == 0) {
      EXPECT_EQ(g, 0);
      continue;
    }
    LL ag = std::llabs(a), bg = std::llabs(b);
    EXPECT_EQ(std::llabs(g), std::gcd(ag, bg));
  }
}

TEST(Exgcd, InverseCoprime) {
  std::mt19937_64 rng(2);
  for (int t = 0; t < 200; ++t) {
    LL m = (LL)(rng() % 50000 + 2);
    LL x = (LL)(rng() % (m - 1) + 1);
    if (std::gcd(x, m) != 1) {
      --t;
      continue;
    }
    LL i = inv(x, m);
    EXPECT_EQ(((__int128)x * i) % m, 1);
  }
  EXPECT_EQ(inv(1, 2), 1);
  EXPECT_EQ(inv(3, 10), 7);
}

TEST(Crt, CoprimeAndNonCoprime) {
  LL A, M;
  ASSERT_TRUE(crt_merge(2, 3, 3, 5, A, M));
  EXPECT_EQ(M, 15);
  EXPECT_EQ(A % 3, 2);
  EXPECT_EQ(A % 5, 3);

  ASSERT_TRUE(crt_merge(0, 4, 2, 6, A, M));
  EXPECT_EQ(A % 4, 0);
  EXPECT_EQ(A % 6, 2);

  EXPECT_FALSE(crt_merge(1, 4, 2, 6, A, M));

  ASSERT_TRUE(crt_merge(0, 1, 0, 1, A, M));
  EXPECT_EQ(A % 1, 0);

  ASSERT_TRUE(crt_merge(5, 10, 15, 20, A, M));
  EXPECT_EQ(A % 10, 5);
  EXPECT_EQ(A % 20, 15);
}

TEST(Crt, ZeroInverseModOne) {
  // After dividing by gcd, m2 can become 1 and inv(0, 1) is documented as
  // "any value". Must succeed, not abort.
  LL A, M;
  ASSERT_TRUE(crt_merge(0, 4, 0, 2, A, M));
  EXPECT_EQ(A % 4, 0);
  EXPECT_EQ(A % 2, 0);
}
