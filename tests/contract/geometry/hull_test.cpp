#include <gtest/gtest.h>
#include "prelude/geo_kernel.hpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/凸包.cpp"

static bool is_ccw_convex(const vp &h) {
  int m = (int)h.size();
  if (m <= 2) return true;
  for (int i = 0; i < m; ++i)
    if (turn(h[i], h[(i + 1) % m], h[(i + 2) % m]) < 0) return false;
  return true;
}

TEST(Hull, Edges) {
  EXPECT_TRUE(convex_hull({}).empty());
  auto h1 = convex_hull({{0, 0}});
  EXPECT_EQ(h1.size(), 1u);
  auto h2 = convex_hull({{0, 0}, {1, 0}});
  EXPECT_EQ(h2.size(), 2u);
  auto col = convex_hull({{0, 0}, {1, 0}, {2, 0}, {3, 0}});
  EXPECT_GE(col.size(), 2u);
  auto sq = convex_hull({{0, 0}, {0, 1}, {1, 0}, {1, 1}, {0, 0}});
  EXPECT_EQ(sq.size(), 4u);
  EXPECT_TRUE(is_ccw_convex(sq));
  auto dups = convex_hull({{0, 0}, {0, 0}, {1, 1}, {1, 1}});
  EXPECT_EQ(dups.size(), 2u);
}

TEST(Hull, SquareIsConvex) {
  auto h = convex_hull({{0, 0}, {0, 2}, {2, 0}, {2, 2}, {1, 1}});
  EXPECT_EQ(h.size(), 4u);
  EXPECT_TRUE(is_ccw_convex(h));
}

TEST(Hull, ContainsAndRandom) {
  mt19937 rng(21);
  for (int t = 0; t < 40; ++t) {
    int n = (int)(rng() % 12) + 1;
    vp p(n);
    for (int i = 0; i < n; ++i)
      p[i] = P((LD)(rng() % 11) - 5, (LD)(rng() % 11) - 5);
    auto h = convex_hull(p);
    EXPECT_TRUE(is_ccw_convex(h));
    for (auto u : p) {
      bool inside = false;
      if (h.size() == 1)
        inside = (u == h[0]);
      else if (h.size() == 2)
        inside = turn(h[0], h[1], u) == 0 &&
                 min(h[0].x, h[1].x) <= u.x && u.x <= max(h[0].x, h[1].x) &&
                 min(h[0].y, h[1].y) <= u.y && u.y <= max(h[0].y, h[1].y);
      else if (h.size() >= 3) {
        inside = true;
        for (int i = 0; i < (int)h.size(); ++i)
          if (turn(h[i], h[(i + 1) % h.size()], u) < 0) inside = false;
      }
      EXPECT_TRUE(inside) << t << " " << (double)u.x << " " << (double)u.y;
    }
  }
}
