#include <gtest/gtest.h>
#include "prelude/common.hpp"

int MOD;
#define power qpow

#include "src/sections/NumberTheory/assets/Math/平方剩余.cpp"


static void check_all_residues(int mod) {
  MOD = mod;
  for (int n = 0; n < mod; ++n) {
    int x = -1;
    bool ok = solve(n, x);
    if (!ok) {
      for (int t = 0; t < mod; ++t) EXPECT_NE((t * t) % mod, n);
    } else {
      EXPECT_GE(x, 0);
      EXPECT_EQ((int)((1LL * x * x) % mod), n);
    }
  }
}

TEST(Tonelli, TinyPrimes) {
  check_all_residues(2);
  check_all_residues(3);
  check_all_residues(5);
  check_all_residues(13);
  check_all_residues(17);
  check_all_residues(97);
}

TEST(Tonelli, ZeroAndTwo) {
  MOD = 13;
  int x;
  ASSERT_TRUE(solve(0, x));
  EXPECT_EQ(x, 0);
  MOD = 2;
  ASSERT_TRUE(solve(1, x));
  EXPECT_EQ(x, 1);
}
