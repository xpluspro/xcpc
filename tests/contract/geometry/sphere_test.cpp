#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/经纬度求球面最短距离.cpp"


TEST(Sphere, Identities) {
  LD R = 1;
  EXPECT_NEAR((double)sphereDis(0, 0, 0, 0, R), 0, 1e-10);
  EXPECT_NEAR((double)sphereDis(0, 0, acos(LD(0)), 0, R), (double)acos(LD(0)), 1e-8);
  LD a = sphereDis(0, 0.3, 1.0, -0.2, 2);
  LD b = sphereDis(1.0, -0.2, 0, 0.3, 2);
  EXPECT_NEAR((double)a, (double)b, 1e-10);
  LD anti = sphereDis(0, 0, 0, 0, 1); // same
  (void)anti;
}
