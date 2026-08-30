#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/DynamicProgramming/assets/yzh/Subset Sum.cpp"


static int dp_subset(const vector<int> &a, int t) {
  vector<char> ok(t + 1);
  ok[0] = 1;
  for (int x : a)
    for (int s = t; s >= x; --s) ok[s] = ok[s] || ok[s - x];
  for (int s = t; s >= 0; --s)
    if (ok[s]) return s;
  return 0;
}

TEST(SubsetSum, VsDP) {
  vector<int> empty;
  EXPECT_EQ(SubsetSum(empty, 0), 0);
  EXPECT_EQ(SubsetSum(empty, 5), 0);
  vector<int> one{5};
  EXPECT_EQ(SubsetSum(one, 0), 0);
  EXPECT_EQ(SubsetSum(one, 5), 5);
  EXPECT_EQ(SubsetSum(one, 4), 0);
  vector<int> a{1, 2, 3, 6};
  EXPECT_EQ(SubsetSum(a, 6), dp_subset(a, 6));
  EXPECT_EQ(SubsetSum(a, 10), dp_subset(a, 10));
  EXPECT_EQ(SubsetSum(a, 11), dp_subset(a, 11));
  std::mt19937 rng(30);
  for (int t = 0; t < 40; ++t) {
    int n = rng() % 6 + 1;
    vector<int> b(n);
    for (int i = 0; i < n; ++i) b[i] = rng() % 8 + 1;
    int T = rng() % 20;
    EXPECT_EQ(SubsetSum(b, T), dp_subset(b, T));
  }
}
