#include <gtest/gtest.h>
#define N 512
#include "prelude/common.hpp"
#include "src/sections/String/assets/String/AC 自动机.cpp"


TEST(Ac, InsertAndGoto) {
  ac = AC();
  ac.ins("ab", 1);
  ac.ins("bc", 2);
  ac.ins("abc", 3);
  ac.build();
  string t = "xabc";
  int x = 0;
  vector<int> hits;
  for (char c : t) {
    x = ac.ch[x][c - 'a'];
    for (int y = x; y; y = ac.fail[y])
      if (ac.tag[y]) hits.push_back(ac.tag[y]);
  }
  EXPECT_FALSE(hits.empty());
  // "ab" and "abc" occur in "xabc"
  EXPECT_NE(find(hits.begin(), hits.end(), 1), hits.end());
  EXPECT_NE(find(hits.begin(), hits.end(), 3), hits.end());
}

TEST(Ac, EmptyAndSingle) {
  ac = AC();
  ac.ins("a", 1);
  ac.build();
  EXPECT_EQ(ac.ch[0]['a' - 'a'] != 0, true);
  EXPECT_EQ(ac.tag[ac.ch[0][0]], 1);
}
