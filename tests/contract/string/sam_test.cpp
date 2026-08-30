#include <gtest/gtest.h>
#define MAXN 256
#include "prelude/common.hpp"
#include "src/sections/String/assets/String/SAM.cpp"
namespace gensam {
#include "src/sections/String/assets/String/广义SAM.cpp"
}


static LL distinct_substrings_sam() {
  LL ans = 0;
  for (int i = 2; i <= sam.tot; ++i) ans += sam.len[i] - sam.len[sam.f[i]];
  return ans;
}

TEST(Sam, DefaultLocalInsert) {
  SAM g;
  g.ins('a');
  g.ins('b');
  EXPECT_GE(g.tot, 2);
}

TEST(Sam, DistinctSubstrings) {
  vector<string> ts = {"a", "aaaa", "abab", "abc", "banana"};
  for (auto t : ts) {
    SAM fresh;
    sam = fresh;
    for (char c : t) sam.ins(c);
    set<string> st;
    for (int i = 0; i < (int)t.size(); ++i)
      for (int j = i; j < (int)t.size(); ++j) st.insert(t.substr(i, j - i + 1));
    EXPECT_EQ(distinct_substrings_sam(), (LL)st.size()) << t;
  }
}

TEST(GenSam, MultipleStrings) {
  gensam::SAM g;
  EXPECT_EQ(g.tot, 1);
  int last = g.ins('a', 1);
  last = g.ins('b', last);
  EXPECT_GE(g.tot, 1);
}
