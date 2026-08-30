#include <gtest/gtest.h>
#define MAXN 256
#include "prelude/common.hpp"

int n;

#include "src/sections/DataStructures/assets/DataStructure/RangeBIT.cpp"


TEST(RangeBit, RangeAddRangeSum) {
  n = 20;
  BIT bit{};
  memset(&bit, 0, sizeof bit);
  vector<ll> a(n + 1);
  auto add = [&](int l, int r, ll k) {
    bit.upd(l, r, k);
    for (int i = l; i <= r; ++i) a[i] += k;
  };
  auto sum = [&](int l, int r) {
    ll s = 0;
    for (int i = l; i <= r; ++i) s += a[i];
    return s;
  };
  add(1, 1, 5);
  EXPECT_EQ(bit.qry(1, 1), 5);
  add(1, n, 1);
  add(3, 7, 10);
  add(7, 7, -3);
  for (int l = 1; l <= n; ++l)
    for (int r = l; r <= n; ++r) EXPECT_EQ(bit.qry(l, r), sum(l, r));
  EXPECT_EQ(bit.qry(1), a[1]);
}
