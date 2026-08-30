#include <gtest/gtest.h>
#include "prelude/common.hpp"

int n;
int f[64][64], m[64][64];
int w(int l, int r) { return (r - l) * (r - l); }

void knuth() {
  for (int i = 1; i <= n; ++i) {
    f[i][i] = 0;
    m[i][i] = i;
    if (i + 1 <= n + 1) f[i + 1][i] = 0;
  }
#include "src/sections/DynamicProgramming/assets/Miscellany/2D1D.cpp"
}


static int naive(int nn) {
  vector<vector<int>> dp(nn + 2, vector<int>(nn + 2, INF));
  for (int i = 1; i <= nn; ++i) dp[i][i] = 0;
  for (int len = 2; len <= nn; ++len)
    for (int l = 1, r = len; r <= nn; ++l, ++r)
      for (int k = l; k < r; ++k)
        dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + (r - l) * (r - l));
  return dp[1][nn];
}

TEST(KnuthYao, VsCubic) {
  for (n = 1; n <= 12; ++n) {
    memset(f, 0, sizeof f);
    memset(m, 0, sizeof m);
    knuth();
    EXPECT_EQ(f[1][n], naive(n)) << n;
  }
}
