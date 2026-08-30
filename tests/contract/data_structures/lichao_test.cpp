#include <gtest/gtest.h>
#define MAXN 1024
#include "prelude/common.hpp"

#define ls (p * 2)
#define rs (p * 2 + 1)

#include "src/sections/DataStructures/assets/DataStructure/李超线段树.cpp"


TEST(Lichao, MaxEnvelope) {
  using namespace Li;
  memset(tag, 0, sizeof tag);
  // lines: y = k*x + b, ids 1..3, domain [1, 10]
  k[1] = 0;
  b[1] = 5;
  k[2] = 1;
  b[2] = -2;
  k[3] = -1;
  b[3] = 12;
  update(1, 10, 1, 10, 1, 1);
  update(1, 10, 1, 10, 1, 2);
  update(1, 10, 1, 10, 1, 3);
  for (int x = 1; x <= 10; ++x) {
    double want = max({0.0 * x + 5, 1.0 * x - 2, -1.0 * x + 12});
    EXPECT_NEAR(query(x, 1, 10, 1), want, 1e-8);
  }
}
