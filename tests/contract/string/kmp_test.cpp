#include <gtest/gtest.h>
#define N 1024
#include "prelude/common.hpp"

int fail[N], z[N], n;
char sbuf[N], b[N];

#include "src/sections/String/assets/String/KMP.cpp"
#include "src/sections/String/assets/String/EXKMP.cpp"


static vector<int> brute_fail(const string &t) {
  int m = (int)t.size();
  vector<int> f(m + 1);
  f[0] = f[1] = 0;
  for (int i = 1; i < m; ++i) {
    int j = f[i];
    while (j && t[i] != t[j]) j = f[j];
    f[i + 1] = (t[i] == t[j] ? j + 1 : 0);
  }
  return f;
}

static vector<int> brute_z(const string &t) {
  int m = (int)t.size();
  vector<int> zz(m + 1);
  zz[1] = m;
  for (int i = 2; i <= m; ++i) {
    while (i - 1 + zz[i] < m && t[zz[i]] == t[i - 1 + zz[i]]) ++zz[i];
  }
  return zz;
}

TEST(Kmp, FailArray) {
  vector<string> ts = {"a", "aaaa", "ababab", "abcabcab", "mississippi"};
  for (auto &t : ts) {
    int m = (int)t.size();
    sbuf[0] = 0;
    for (int i = 0; i < m; ++i) sbuf[i + 1] = t[i];
    sbuf[m + 1] = 0;
    kmp(sbuf, m);
    auto w = brute_fail(t);
    for (int i = 1; i <= m; ++i) EXPECT_EQ(fail[i], w[i]) << t << " " << i;
  }
}

TEST(Exkmp, ZArray) {
  vector<string> ts = {"a", "aaaa", "ababab", "abcabcabc", "zabcabd"};
  for (auto &t : ts) {
    int m = (int)t.size();
    char buf[N];
    buf[0] = 0;
    for (int i = 0; i < m; ++i) buf[i + 1] = t[i];
    int a[N];
    exkmp(buf, a, m);
    auto w = brute_z(t);
    for (int i = 1; i <= m; ++i) EXPECT_EQ(a[i], w[i]) << t << " " << i;
  }
}

TEST(ExkmpSnippetInit, ZOnB) {
  string t = "ababab";
  n = (int)t.size();
  b[0] = 0;
  for (int i = 0; i < n; ++i) b[i + 1] = t[i];
  init();
  EXPECT_EQ(z[1], 0);
  for (int i = 2; i <= n; ++i) {
    int br = 0;
    while (i + br <= n && b[i + br] == b[1 + br]) ++br;
    EXPECT_EQ(z[i], br);
  }
}
