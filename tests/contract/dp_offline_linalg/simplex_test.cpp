#include <gtest/gtest.h>
#include "prelude/common.hpp"
#undef INF

#define main xcpc_snippet_main
#include "src/sections/LinearAlgebra/assets/Math/Simplex.cpp"
#undef main


TEST(Simplex, Feasible) {
  // max x1 + x2 s.t. x1+x2 <= 2, xi>=0
  memset(a, 0, sizeof a);
  n = 2;
  m = 1;
  a[0][1] = 1;
  a[0][2] = 1;
  a[1][1] = -1;
  a[1][2] = -1;
  a[1][0] = 2;
  ASSERT_TRUE(solve());
  EXPECT_NEAR((double)ans(), 2.0, 1e-5);
}

TEST(Simplex, Infeasible) {
  memset(a, 0, sizeof a);
  n = 1;
  m = 2;
  a[0][1] = 1;
  a[1][1] = -1;
  a[1][0] = -1; // x <= -1 impossible with x>=0
  a[2][1] = 0;
  a[2][0] = 0;
  testing::internal::CaptureStdout();
  bool ok = solve();
  testing::internal::GetCapturedStdout();
  EXPECT_FALSE(ok);
}

TEST(Simplex, Unbounded) {
  memset(a, 0, sizeof a);
  n = 1;
  m = 1;
  a[0][1] = 1;
  a[1][1] = 1; // -x1?  Ax <= b with A = -1? maximize x, constraint x>=0 only
  a[1][0] = 0;
  a[1][1] = 1; // 1*x <= 0? let's do no upper bound: m=0
  m = 0;
  testing::internal::CaptureStdout();
  bool ok = solve();
  testing::internal::GetCapturedStdout();
  EXPECT_FALSE(ok);
}
