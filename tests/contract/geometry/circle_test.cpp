#include <gtest/gtest.h>
#include "prelude/geo_kernel.hpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/8.4-circle.cpp"

TEST(Circle, MinCircleSmall) {
  EXPECT_NEAR((double)min_circle({}).r, 0, 1e-9);
  auto c1 = min_circle({{3, 4}});
  EXPECT_NEAR((double)c1.r, 0, 1e-9);
  auto c2 = min_circle({{0, 0}, {2, 0}});
  EXPECT_NEAR((double)c2.r, 1, 1e-8);
  vp p = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
  auto c = min_circle(p);
  for (auto u : p) EXPECT_TRUE(in_circle(u, c));
  EXPECT_LE((double)c.r, 0.8);
}

TEST(Circle, IntersectArea) {
  C a{{0, 0}, 1}, b{{0, 0}, 1};
  EXPECT_NEAR((double)cc_intersection_area(a, b), (double)pi, 1e-6);
  C d{{3, 0}, 1};
  EXPECT_NEAR((double)cc_intersection_area(a, d), 0, 1e-9);
  auto pts = cc_intersection({{0, 0}, 1}, {{2, 0}, 1});
  EXPECT_EQ(pts.size(), 1u);
  auto none = cc_intersection({{0, 0}, 1}, {{0, 0}, 2});
  EXPECT_TRUE(none.empty());
}
