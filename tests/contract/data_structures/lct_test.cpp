#include <gtest/gtest.h>
#define N 256
#include "prelude/common.hpp"

int siz[N], F[N], G[N];
multiset<int> s[N];

#include "adapters/lct_basic.hpp"


TEST(Lct, LinkCutReachPathMax) {
  using namespace LCT;
  int n = 6;
  for (int i = 1; i <= n; ++i) {
    val[i] = i;
    ans[i] = i;
    c[i][0] = c[i][1] = f[i] = rev[i] = 0;
  }
  link(1, 2);
  link(2, 3);
  link(4, 5);
  EXPECT_TRUE(reach(1, 3));
  EXPECT_FALSE(reach(1, 4));
  split(1, 3);
  EXPECT_EQ(ans[3], 3);
  link(3, 4);
  EXPECT_TRUE(reach(1, 5));
  cut(2, 3);
  EXPECT_FALSE(reach(1, 5));
  EXPECT_TRUE(reach(3, 5));
  // link already-connected is a no-op
  link(4, 5);
  EXPECT_TRUE(reach(4, 5));
}
