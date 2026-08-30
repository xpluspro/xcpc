#include <gtest/gtest.h>
#define N 1024
#include "prelude/common.hpp"

char s[N];
int n, h[N];

#include "src/sections/String/assets/String/Manacher.cpp"


static void load(const string &t) {
  n = 0;
  s[0] = '~';
  s[++n] = '|';
  for (char c : t) {
    s[++n] = c;
    s[++n] = '|';
  }
  s[n + 1] = 0;
}

TEST(Manacher, RadiiMatchBrute) {
  vector<string> ts = {"", "a", "aa", "aba", "abba", "abc", "aaaa", "ababab"};
  for (auto &t : ts) {
    load(t);
    memset(h, 0, sizeof h);
    manacher();
    for (int i = 1; i <= n; ++i) {
      int r = 1;
      while (i - r >= 0 && i + r <= n + 1 && s[i - r] == s[i + r]) ++r;
      // manacher stores radius with s[i-h]==s[i+h] already failed, so h is
      // the first failing offset; brute `r` is that failing offset.
      EXPECT_EQ(h[i], r) << t << " " << i;
    }
  }
}
