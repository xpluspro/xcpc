#include <gtest/gtest.h>
#define MAXN 32
#include "prelude/common.hpp"

int n;
vector<int> e[MAXN];

#include "src/sections/GraphTheory/assets/graph/Blossom_Tree.cpp"


static int brute_match(int nv, const vector<pair<int, int>> &ed) {
  int m = (int)ed.size(), best = 0;
  for (int mask = 0; mask < (1 << m); ++mask) {
    vector<int> used(nv + 1);
    int cnt = 0;
    bool ok = true;
    for (int i = 0; i < m; ++i)
      if (mask >> i & 1) {
        auto [a, b] = ed[i];
        if (used[a] || used[b]) {
          ok = false;
          break;
        }
        used[a] = used[b] = 1;
        ++cnt;
      }
    if (ok) best = max(best, cnt);
  }
  return best;
}

TEST(Blossom, LcaUnmatchedReturnsZero) {
  for (int i = 0; i < MAXN; ++i) fa[i] = i, mch[i] = 0, pre[i] = 0, hs[i] = 0;
  EXPECT_EQ(lca(0, 0), 0);
}

TEST(Blossom, VsBrute) {
  vector<vector<pair<int, int>>> gs = {
      {{1, 2}},
      {{1, 2}, {2, 3}, {3, 1}},
      {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 3}},
      {{1, 2}, {3, 4}},
  };
  for (auto &ed : gs) {
    n = 0;
    for (auto [a, b] : ed) n = max({n, a, b});
    for (int i = 0; i <= n; ++i) e[i].clear(), mch[i] = 0, pre[i] = 0;
    for (auto [a, b] : ed) {
      e[a].push_back(b);
      e[b].push_back(a);
    }
    int got = solve();
    EXPECT_EQ(got, brute_match(n, ed));
  }
}
