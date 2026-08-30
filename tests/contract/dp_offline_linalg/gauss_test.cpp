#include <gtest/gtest.h>
#include "prelude/common.hpp"

const LD eps = 1e-9;

#include "src/sections/LinearAlgebra/assets/Geometry/minnorm_gauss.cpp"


TEST(Gauss, FullRank) {
  // x+y=1, x-y=1 => x=1,y=0
  vector<vec> a = {{1, 1, -1}, {1, -1, -1}};
  auto [x, t] = gauss(a, 2, 2);
  EXPECT_NEAR((double)x[0], 1, 1e-6);
  EXPECT_NEAR((double)x[1], 0, 1e-6);
}

TEST(Gauss, LeastNormUnderdetermined) {
  // x + y = 2, one equation two vars. min-norm is (1,1)
  vector<vec> a = {{1, 1, -2}};
  auto [x, t] = gauss(a, 1, 2);
  EXPECT_NEAR((double)x[0], 1, 1e-5);
  EXPECT_NEAR((double)x[1], 1, 1e-5);
}
