#include <gtest/gtest.h>
#include "prelude/geo_kernel.hpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/半平面交.cpp"

static vector<L> box(LD B) {
  return {{{-B, -B}, {B, -B}}, {{B, -B}, {B, B}}, {{B, B}, {-B, B}}, {{-B, B}, {-B, -B}}};
}

TEST(Hpi, SquareAndHalf) {
  auto h = box(10);
  auto p = hpi(h);
  EXPECT_EQ(p.size(), 4u);
  h.push_back({{0, -10}, {0, 10}});
  auto q = hpi(h);
  EXPECT_GE(q.size(), 3u);
}

TEST(Hpi, EmptyWithBox) {
  auto h = box(1);
  h.push_back({{5, 0}, {5, 1}});
  h.push_back({{6, 1}, {6, 0}});
  auto p = hpi(h);
  (void)p;
}
