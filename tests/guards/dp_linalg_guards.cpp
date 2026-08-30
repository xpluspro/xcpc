#include "prelude/common.hpp"
#include "prelude/death.hpp"
#include "src/sections/DynamicProgramming/assets/yzh/Subset Sum.cpp"

const LD eps = 1e-9;
#include "src/sections/LinearAlgebra/assets/Geometry/minnorm_gauss.cpp"

TEST(Guards, GaussWellPosedDoesNotAbort) {
  vector<vec> a = {{1, 1, -1}, {1, -1, -1}};
  auto [x, t] = gauss(a, 2, 2);
  EXPECT_NEAR((double)x[0], 1, 1e-6);
}
