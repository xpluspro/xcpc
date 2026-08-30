#include <gtest/gtest.h>
#define MAXN 256
#include "prelude/common.hpp"

int n;

#include "src/sections/DataStructures/assets/DataStructure/左偏树.cpp"


TEST(Leftist, MergeDel) {
  using namespace Leftist;
  n = 5;
  memset(ls, 0, sizeof ls);
  memset(rs, 0, sizeof rs);
  memset(dis, 0, sizeof dis);
  memset(vis, 0, sizeof vis);
  dis[0] = -1;
  for (int i = 1; i <= n; ++i) a[i] = i;
  init();
  merge(1, 2);
  merge(3, 4);
  merge(1, 3);
  int x = del(1);
  EXPECT_TRUE(x == 1 || x == 2 || x == 3 || x == 4);
  EXPECT_EQ(del(1), -1);
}
