#include <gtest/gtest.h>
#include "prelude/common.hpp"

#define main xcpc_snippet_main
#include "src/sections/String/assets/String/SA.cpp"
#undef main


static int lcp_brute(const char *s, int n, int i, int j) {
  int k = 0;
  while (i + k <= n && j + k <= n && s[i + k] == s[j + k]) ++k;
  return k;
}

TEST(Sa, SortedAndHeight) {
  vector<string> ts = {"a", "aaaa", "banana", "mississippi", "abcabxabcd"};
  for (auto t : ts) {
    int n = (int)t.size();
    for (int i = 1; i <= n; ++i) s[i] = t[i - 1];
    s[n + 1] = 0;
    memset(sa, 0, sizeof(int) * (n + 5));
    memset(rnk, 0, sizeof(int) * (n + 5));
    memset(height, 0, sizeof(int) * (n + 5));
    get_sa(s, n, sa, rnk, height);
    for (int i = 1; i < n; ++i) {
      string a = t.substr(sa[i] - 1);
      string b = t.substr(sa[i + 1] - 1);
      EXPECT_LT(a, b);
    }
    for (int i = 2; i <= n; ++i)
      EXPECT_EQ(height[i], lcp_brute(s, n, sa[i], sa[i - 1]));
  }
}

TEST(Sa, MultiTestReset) {
  string t1 = "abc";
  string t2 = "zzz";
  auto run = [](const string &t) {
    int n = (int)t.size();
    memset(sa, 0, sizeof(int) * 16);
    memset(rnk, 0, sizeof(int) * 16);
    memset(height, 0, sizeof(int) * 16);
    memset(s, 0, 16);
    for (int i = 1; i <= n; ++i) s[i] = t[i - 1];
    get_sa(s, n, sa, rnk, height);
    for (int i = 1; i < n; ++i)
      EXPECT_LT(t.substr(sa[i] - 1), t.substr(sa[i + 1] - 1));
  };
  run(t1);
  run(t2);
}
