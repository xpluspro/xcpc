#include <gtest/gtest.h>
#define N 1024
#define MAXN 1024
#include "prelude/common.hpp"

int a[MAXN];
void push_down(int) {}
int rd() {
  static std::mt19937 rng(11);
  return (int)rng();
}

#include "src/sections/DataStructures/assets/DataStructure/FHQ_treap.cpp"


TEST(Fhq, VsMultiset) {
  tr = FHQ();
  multiset<int> s;
  std::mt19937 rng(12);
  for (int t = 0; t < 200; ++t) {
    int op = rng() % 5;
    int v = (int)(rng() % 30);
    if (op == 0 || s.empty()) {
      tr.ins(v);
      s.insert(v);
    } else if (op == 1) {
      auto it = s.find(v);
      if (it != s.end()) {
        tr.del(v);
        s.erase(it);
      }
    } else if (op == 2) {
      int r = 0;
      for (int x : s) {
        ++r;
        if (x >= v) break;
      }
      if (s.empty() || *s.begin() >= v) {
        // rank of v
      }
      EXPECT_EQ(tr.rk(v), (int)distance(s.begin(), s.lower_bound(v)) + 1);
    } else if (op == 3) {
      int k = rng() % (int)s.size() + 1;
      auto it = s.begin();
      for (int i = 1; i < k; ++i) ++it;
      EXPECT_EQ(tr.kth(tr.rt, k), *it);
    }
  }
}

TEST(Fhq, BuildPreservesSequence) {
  // build() constructs an implicit-key treap from a[1..n], not a BST on values.
  tr = FHQ();
  vector<int> vals = {4, 2, 5, 1, 3};
  for (int i = 1; i <= 5; ++i) a[i] = vals[i - 1];
  tr.rt = tr.build(5);
  for (int i = 1; i <= 5; ++i) EXPECT_EQ(tr.kth(tr.rt, i), vals[i - 1]);
}

TEST(Fhq, RankKthPreNxt) {
  tr = FHQ();
  for (int x : {3, 1, 4, 1, 5, 9, 2}) tr.ins(x);
  EXPECT_EQ(tr.rk(1), 1);
  EXPECT_EQ(tr.rk(4), 5);
  EXPECT_EQ(tr.kth(tr.rt, 1), 1);
  EXPECT_EQ(tr.kth(tr.rt, 7), 9);
  EXPECT_EQ(tr.pre(4), 3);
  EXPECT_EQ(tr.nxt(4), 5);
  tr.del(1);
  EXPECT_EQ(tr.kth(tr.rt, 1), 1);
}


