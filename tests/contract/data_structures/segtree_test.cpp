#include <gtest/gtest.h>
#define M 128
#include "prelude/common.hpp"

int t[M * 4], mark[M * 4];

#include "src/sections/DataStructures/assets/DataStructure/非递归线段树求最大值.cpp"


TEST(Segtree, RangeAddRangeMax) {
  memset(t, 0, sizeof t);
  memset(mark, 0, sizeof mark);
  int n = 16;
  vector<int> a(n + 1);
  auto bmax = [&](int l, int r) {
    int m = -INF;
    for (int i = l; i <= r; ++i) m = max(m, a[i]);
    return m;
  };
  update(1, n, 0); // no-op style init not required
  for (int i = 1; i <= n; ++i) {
    update(i, i, i);
    a[i] = i;
  }
  EXPECT_EQ(query(1, n), n);
  EXPECT_EQ(query(3, 5), 5);
  update(2, 8, 10);
  for (int i = 2; i <= 8; ++i) a[i] += 10;
  for (int l = 1; l <= n; ++l)
    for (int r = l; r <= n; ++r) EXPECT_EQ(query(l, r), bmax(l, r));
  update(1, 1, 1000);
  a[1] += 1000;
  EXPECT_EQ(query(1, 1), a[1]);
}
