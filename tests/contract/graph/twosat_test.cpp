#include <gtest/gtest.h>
#define N 64
#include "prelude/common.hpp"

int n;
vector<int> E[N];

#include "src/sections/GraphTheory/assets/graph/2-sat.cpp"


TEST(TwoSat, SatAndUnsat) {
  auto reset = [] {
    for (int i = 0; i < N; ++i) E[i].clear();
    memset(dfn, 0, sizeof dfn);
    memset(scc, 0, sizeof scc);
    stp = top = sccs = 0;
  };
  reset();
  n = 1;
  add(0, 1, 0, 1); // x0 true => x0 true
  EXPECT_TRUE(solve());

  reset();
  n = 1;
  add(0, 1, 0, 0);
  add(0, 0, 0, 1); // x and not x
  EXPECT_FALSE(solve());

  reset();
  n = 2;
  // (x0 or x1) = (~x0 => x1) and (~x1 => x0)
  add(0, 0, 1, 1);
  add(1, 0, 0, 1);
  EXPECT_TRUE(solve());
}
