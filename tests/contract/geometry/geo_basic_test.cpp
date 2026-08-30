#include <gtest/gtest.h>
#include "prelude/geo_kernel.hpp"

TEST(Geo, ProjectionAndSegment) {
  L l{{0, 0}, {2, 0}};
  P p{1, 1};
  auto pr = project_to_line(p, l);
  EXPECT_NEAR((double)pr.x, 1.0, 1e-9);
  EXPECT_NEAR((double)pr.y, 0.0, 1e-9);
  EXPECT_TRUE(point_on_segment({1, 0}, l));
  EXPECT_TRUE(point_on_segment({0, 0}, l));
  EXPECT_TRUE(point_on_segment({2, 0}, l));
  EXPECT_FALSE(point_on_segment({3, 0}, l));
  EXPECT_NEAR((double)point_to_segment({1, 1}, l), 1.0, 1e-9);
  EXPECT_NEAR((double)point_to_segment({3, 0}, l), 1.0, 1e-9);
  L d{{5, 5}, {5, 5}};
  EXPECT_NEAR((double)point_to_segment({5, 6}, d), 1.0, 1e-9);
}

TEST(Geo, Intersection) {
  L a{{0, 0}, {1, 1}}, b{{0, 1}, {1, 0}};
  EXPECT_TRUE(intersection_judge(a, b));
  auto q = ll_intersection(a, b);
  EXPECT_NEAR((double)q.x, 0.5, 1e-9);
  EXPECT_NEAR((double)q.y, 0.5, 1e-9);
  L c{{0, 0}, {2, 0}}, e{{1, 0}, {3, 0}};
  EXPECT_TRUE(intersection_judge(c, e));
  EXPECT_TRUE(point_on_ray({2, 2}, {{0, 0}, {1, 1}}));
  EXPECT_FALSE(point_on_ray({-1, -1}, {{0, 0}, {1, 1}}));
}

TEST(Geo, Rot) {
  P r = P{1, 0}.rot90();
  EXPECT_NEAR((double)r.x, 0, 1e-9);
  EXPECT_NEAR((double)r.y, 1, 1e-9);
}
