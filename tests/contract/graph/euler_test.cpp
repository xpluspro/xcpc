#include <gtest/gtest.h>
#define N 32
#define M 64
#include "prelude/common.hpp"

int n;

#include "src/sections/GraphTheory/assets/graph/欧拉回路.cpp"


TEST(Euler, UndirectedCircuit) {
  auto reset = [] {
    e = 0;
    for (int i = 0; i < N; ++i) E[i].clear(), cur[i] = 0;
    memset(id, 0, sizeof id);
    memset(vis, 0, sizeof vis);
    while (!stk.empty()) stk.pop();
  };
  reset();
  n = 3;
  add(1, 2);
  add(2, 3);
  add(3, 1);
  EXPECT_TRUE(valid());
  dfs(1);
  EXPECT_EQ((int)stk.size(), 3);

  reset();
  n = 3;
  add(1, 2);
  add(2, 3);
  EXPECT_FALSE(valid());
}
