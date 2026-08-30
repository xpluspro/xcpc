#include <gtest/gtest.h>
#define N 32
#include "prelude/common.hpp"

int n;

#include "src/sections/GraphTheory/assets/graph/kosaraju.cpp"


TEST(Kosaraju, Partition) {
  auto run = [](int nv, vector<pair<int, int>> eds) {
    n = nv;
    for (int i = 0; i < N; ++i) e[i].reset(), re[i].reset();
    vis.reset();
    sta.clear();
    for (auto [u, v] : eds) {
      e[u].set(v);
      re[v].set(u);
    }
    return solve();
  };
  auto one = run(1, {});
  EXPECT_EQ(one.size(), 1u);
  auto cyc = run(3, {{1, 2}, {2, 3}, {3, 1}});
  EXPECT_EQ(cyc.size(), 1u);
  auto line = run(3, {{1, 2}, {2, 3}});
  EXPECT_EQ(line.size(), 3u);
}
