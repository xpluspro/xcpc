#include <gtest/gtest.h>
#define N 64
#include "prelude/common.hpp"
#include "src/sections/GraphTheory/assets/graph/Hungarian.cpp"

namespace hk {
#include "src/sections/GraphTheory/assets/graph/hk_skip2004.cpp"
}


static int brute_bip(int nl, int nr, const vector<pair<int, int>> &ed) {
  int m = (int)ed.size(), best = 0;
  for (int mask = 0; mask < (1 << m); ++mask) {
    vector<int> L(nl + 1), R(nr + 1);
    int cnt = 0;
    bool ok = true;
    for (int i = 0; i < m; ++i)
      if (mask >> i & 1) {
        auto [a, b] = ed[i];
        if (L[a] || R[b]) {
          ok = false;
          break;
        }
        L[a] = R[b] = 1;
        ++cnt;
      }
    if (ok) best = max(best, cnt);
  }
  return best;
}

TEST(Matching, HungarianAndHK) {
  vector<vector<pair<int, int>>> graphs = {
      {{1, 1}},
      {{1, 1}, {1, 2}, {2, 2}},
      {{1, 1}, {2, 1}, {2, 2}, {3, 3}},
      {{1, 1}, {2, 2}, {3, 1}, {3, 2}},
  };
  for (auto &ed : graphs) {
    int nl = 0, nr = 0;
    for (auto [a, b] : ed) nl = max(nl, a), nr = max(nr, b);
    for (int i = 0; i < N; ++i) {
      edge[i].reset();
      hk::E[i].clear();
    }
    for (auto [a, b] : ed) {
      edge[a].set(b);
      hk::E[a].push_back(b);
    }
    auto mh = match(nl, nr);
    int got = 0;
    for (int i = 1; i <= nl; ++i)
      if (mh[i]) ++got;
    int want = brute_bip(nl, nr, ed);
    EXPECT_EQ(got, want);
    hk::match(nl, nr);
    int g2 = 0;
    for (int i = 1; i <= nl; ++i)
      if (hk::ml[i]) ++g2;
    EXPECT_EQ(g2, want);
  }
}
