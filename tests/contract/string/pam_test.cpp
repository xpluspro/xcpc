#include <gtest/gtest.h>
#define N 512
#include "prelude/common.hpp"

char s[N];

#include "src/sections/String/assets/String/PAM.cpp"


TEST(Pam, DistinctPalindromesBoundAndCount) {
  vector<string> ts = {"a", "aaaa", "aba", "abba", "abc", "aaaabaaa"};
  for (auto t : ts) {
    pam = PAM();
    s[0] = 0;
    for (int i = 0; i < (int)t.size(); ++i) s[i + 1] = t[i];
    s[t.size() + 1] = 0;
    pam.build();
    set<string> pals;
    for (int i = 0; i < (int)t.size(); ++i)
      for (int j = i; j < (int)t.size(); ++j) {
        string u = t.substr(i, j - i + 1);
        string r = u;
        reverse(r.begin(), r.end());
        if (u == r) pals.insert(u);
      }
    // tot-1 nodes besides the two roots 0 and 1 are palindromes
    EXPECT_EQ(pam.tot - 1, (int)pals.size()) << t;
  }
}
