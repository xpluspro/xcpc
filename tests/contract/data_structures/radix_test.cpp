#include <gtest/gtest.h>
#define MAXN 512
#include "prelude/common.hpp"

int b[1 << 8], c[MAXN];

#include "src/sections/DataStructures/assets/Miscellany/RadixSort.cpp"


TEST(Radix, VsStdSort) {
  auto run = [](vector<int> v) {
    auto want = v;
    sort(want.begin(), want.end());
    int n = (int)v.size();
    int a[MAXN];
    for (int i = 1; i <= n; ++i) a[i] = v[i - 1];
    if (n) Sort(a, n);
    for (int i = 1; i <= n; ++i) EXPECT_EQ(a[i], want[i - 1]);
  };
  run({});
  run({0});
  run({0, 1, 2});
  run({5, 5, 5});
  run({(int)0x7fffffff, 0, 1, 2});
  std::mt19937 rng(13);
  for (int t = 0; t < 40; ++t) {
    int n = rng() % 200 + 1;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = (int)(rng() & 0x7fffffff);
    run(a);
  }
}
