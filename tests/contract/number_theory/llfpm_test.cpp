#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Miscellany/LLFPM.cpp"


static LL naive_modmul(LL a, LL b, LL m) {
  LL r = (LL)((__int128)a * b % m);
  if (r < 0) r += m;
  return r;
}

TEST(Llfpm, Signed63Bit) {
  std::mt19937_64 rng(3);
  vector<LL> mods = {2, 3, 7, (1LL << 31) - 1, 1000000007LL, (1LL << 61) - 1};
  for (LL m : mods) {
    EXPECT_EQ(modmul(0LL, 0LL, m), 0);
    EXPECT_EQ(modmul(1LL, 1LL, m), 1 % m);
    EXPECT_EQ(modmul(m - 1, m - 1, m), naive_modmul(m - 1, m - 1, m));
    for (int t = 0; t < 80; ++t) {
      LL a = (LL)(rng() % (ULL)m);
      LL b = (LL)(rng() % (ULL)m);
      EXPECT_EQ(modmul(a, b, m), naive_modmul(a, b, m));
    }
  }
}

TEST(Llfpm, UnsignedAndBarrett) {
  std::mt19937_64 rng(4);
  LL m = 1000000007;
  for (int t = 0; t < 100; ++t) {
    ULL a = rng() % (ULL)m, b = rng() % (ULL)m;
    EXPECT_EQ((LL)modmul(a, b, m), naive_modmul((LL)a, (LL)b, m));
  }
  DIV d;
  d.init((ULL)m);
  for (int t = 0; t < 100; ++t) {
    ULL x = rng();
    EXPECT_EQ(d.mod(x), (int)(x % (ULL)m));
  }
  EXPECT_EQ(d.mod(0), 0);
  EXPECT_EQ(d.mod((ULL)m), 0);
}
