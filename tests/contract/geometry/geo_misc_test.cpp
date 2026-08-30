#include <gtest/gtest.h>
#include <set>
#include "prelude/geo_kernel.hpp"

using LL = long long;
template <class T>
T gcd_signed(T a, T b) {
  if (a < 0) a = -a;
  if (b < 0) b = -b;
  while (b) {
    T t = a % b;
    a = b;
    b = t;
  }
  return a;
}
#define __gcd gcd_signed

#include "src/sections/ComputationalGeometry/assets/Geometry/8.4-circle.cpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/三角形.cpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/圆上整点.cpp"
#include "src/sections/ComputationalGeometry/assets/Math/Simpson.cpp"

TEST(Triangle, Centers) {
  P A{0, 0}, B{4, 0}, C{0, 3};
  auto ic = incenter(A, B, C);
  EXPECT_GT((double)ic.x, 0);
  EXPECT_GT((double)ic.y, 0);
  auto o = circumcenter(A, B, C);
  EXPECT_NEAR((double)(o - A).len(), (double)(o - B).len(), 1e-8);
  EXPECT_NEAR((double)(o - A).len(), (double)(o - C).len(), 1e-8);
  auto oc = orthocenter(A, B, C);
  EXPECT_NEAR((double)oc.x, 0, 1e-6);
  EXPECT_NEAR((double)oc.y, 0, 1e-6);
}

TEST(LatticeCircle, SmallR) {
  for (LL r : {1, 2, 5, 13}) {
    auto ys = solve(r);
    std::set<LL> got(ys.begin(), ys.end());
    std::set<LL> want;
    for (LL y = 0; y <= r; ++y)
      for (LL x = 0; x <= r; ++x)
        if (x * x + y * y == r * r) want.insert(y);
    EXPECT_EQ(got, want) << r;
  }
}

static LD quad(LD x) { return x * x; }
static LD sine(LD x) { return sin((double)x); }

TEST(Simpson, PolynomialAndSin) {
  simpson S;
  EXPECT_NEAR((double)S.solve(quad, 0, 1, 1e-10), 1.0 / 3, 1e-7);
  EXPECT_NEAR((double)S.solve(sine, 0, pi, 1e-8), 2.0, 1e-5);
}
