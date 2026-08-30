#include "prelude/common.hpp"
#include "prelude/death.hpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/经纬度求球面最短距离.cpp"

TEST(Guards, SphereBadRadius) { XCPC_EXPECT_ASSERT((void)sphereDis(0, 0, 0, 0, 0)); }

TEST(Guards, SphereBadLat) {
  XCPC_EXPECT_ASSERT((void)sphereDis(0, 2, 0, 0, 1));
}

TEST(Guards, SphereBadLon) {
  XCPC_EXPECT_ASSERT((void)sphereDis(4, 0, 0, 0, 1));
}
