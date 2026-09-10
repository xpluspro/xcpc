#include <bits/stdc++.h>
using namespace std;
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.1-declarations.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/halfplane_feasible_closed.cpp"

// 小整数输入的精确 oracle：最近点只能是原点、某条线的垂足或两线交点。
// 用有理数候选和 __int128 叉乘判定，不依赖浮点 HPI 或随机增量。
bool exact_feasible(const vector<L>& lines) {
  using I = __int128_t;
  struct Inequality { I a, b, c; };
  vector<Inequality> inequalities;
  for (cl line : lines) {
    I sx = (long long)line.s.x, sy = (long long)line.s.y;
    I a = sy - (long long)line.t.y, b = (long long)line.t.x - sx;
    inequalities.push_back({a, b, a * sx + b * sy});
  }
  auto valid = [&](I x, I y, I denominator) {
    if (denominator < 0) x = -x, y = -y, denominator = -denominator;
    for (auto row : inequalities)
      if (row.a * x + row.b * y < row.c * denominator) return false;
    return true;
  };
  if (valid(0, 0, 1)) return true;
  for (auto row : inequalities) {
    I norm = row.a * row.a + row.b * row.b;
    if (norm && valid(row.a * row.c, row.b * row.c, norm)) return true;
    for (auto other : inequalities) {
      I determinant = row.a * other.b - row.b * other.a;
      if (determinant && valid(row.c * other.b - row.b * other.c,
          row.a * other.c - row.c * other.a, determinant)) return true;
    }
  }
  return false;
}

int main() {
  rnd.seed(20260910);
  // 用户回归：2 1 19 / 10 7 7，含非负 A、B，解退化到线段。
  vector<L> regression = {{{0,1},{0,0}}, {{0,0},{1,0}}};
  for (auto [x,c,d] : vector<array<LD,3>>{{2,1,19},{10,7,7}}) {
    regression.push_back(L({0,c/x},{1,c/x-x}));
    regression.push_back(L({1,d/x-x},{0,d/x}));
  }
  for (int seed = 0; seed < 10000; ++seed) {
    rnd.seed(seed);
    assert(halfplane_feasible_closed(regression));
  }
  vector<ClosedHalfplane> rows = {
    {{1,0},0}, {{0,1},0}, {{4,2},1}, {{-4,-2},-19},
    {{100,10},7}, {{-100,-10},-7}
  };
  auto point = rows;
  point.push_back({{1,0},0.01L});
  point.push_back({{-1,0},-0.01L}); // 唯一解 (0.01, 0.6)
  auto impossible = point;
  impossible.push_back({{0,1},0.61L});
  auto scaled_impossible = impossible;
  for (auto& row : scaled_impossible) {
    row.normal = row.normal * 1e20L;
    row.bound *= 1e20L;
  }
  for (int seed = 0; seed < 10000; ++seed) {
    rnd.seed(seed);
    assert(halfplane_feasible_closed_coefficients(rows));
    assert(halfplane_feasible_closed_coefficients(point));
    assert(!halfplane_feasible_closed_coefficients(impossible));
    assert(!halfplane_feasible_closed_coefficients(scaled_impossible));
  }
  assert(halfplane_feasible_closed_coefficients({}));
  assert(halfplane_feasible_closed_coefficients({{{0,0},0},{{0,0},-1}}));
  assert(!halfplane_feasible_closed_coefficients({{{0,0},1}}));
  // 方向不经过端点相减：很大的截距也保留原始平行关系。
  assert(!halfplane_feasible_closed_coefficients({
    {{10,1},1e18L}, {{-10,-1},-1e18L+100}}));
  // 非零的小夹角不能简单当平行：交点在远处。
  assert(halfplane_feasible_closed_coefficients({
    {{0,1},1}, {{1e-20L,-1},0}}));
  vector<pair<vector<L>, bool>> cases = {
    {{}, true},
    {{{{5,5},{5,5}}}, true}, // 零方向按无约束处理
    {{{{0,1},{1,1}}}, true}, // 无界半平面
    {{{{0,1},{1,1}},{{1,2},{0,2}}}, true}, // 无界条带
    {{{{0,1},{1,1}},{{1,1},{0,1}}}, true}, // 整条直线
    {{{{0,1},{1,1}},{{1,0},{0,0}}}, false}, // 平行矛盾
    {{{{0,0},{1,0}},{{1,0},{0,0}},
       {{0,1},{0,0}},{{1,0},{1,1}}}, true}, // 线段
    {{{{0,0},{1,0}},{{0,1},{0,0}},{{1,0},{0,1}}}, true}, // 三角形
    {{{{0,0},{1,0}},{{0,1},{0,0}},{{0,0},{-1,1}}}, true}, // 单点
    {{{{0,0},{1,0}},{{0,1},{0,0}},{{-1,0},{-2,1}}}, false},
    {{{{0,1},{1,0}},{{1,0},{0,1}},
       {{0,0},{1,1}},{{1,1},{0,0}}}, true}, // 非轴对齐单点
    {{{{0,1},{1,1}},{{0,0},{-1,-1e-14L}}}, true}, // 可行点 x >= 1e14
    {{{{2e9L,0},{2e9L,-1}},{{2e9L,0},{2e9L,1}}}, true},
    {{{{8,7},{-9,-10}},{{2,7},{0,3}},
       {{2,1},{-6,-7}},{{-4,-2},{-6,2}}}, true}, // 同边界不同倍数舍入回归
  };
  for (const auto& [lines, expected] : cases)
    for (int repeat = 0; repeat < 100; ++repeat) {
      assert(halfplane_feasible_closed(lines) == expected);
      assert(halfplane_feasible_closed(lines, eps) == expected);
    }

  // 容差为距离，与有向边长度无关；零容差不主动放宽边界。
  vector<L> tiny_gap = {{{0,0},{1,0}},{{1,-eps},{0,-eps}}};
  assert(halfplane_feasible_closed(tiny_gap, eps));
  assert(!halfplane_feasible_closed(tiny_gap));
  tiny_gap[0].t = P(1e6L,0);
  assert(halfplane_feasible_closed(tiny_gap, eps));
  vector<L> larger_gap = {{{0,0},{1,0}},{{1,-4*eps},{0,-4*eps}}};
  assert(!halfplane_feasible_closed(larger_gap, eps));

  mt19937 generator(987654321);
  uniform_int_distribution<int> coordinate(-10,10), count(0,14);
  for (int trial = 0; trial < 20000; ++trial) {
    vector<L> lines;
    int size = count(generator);
    for (int i = 0; i < size; ++i) {
      P start(coordinate(generator), coordinate(generator));
      P finish(coordinate(generator), coordinate(generator));
      lines.push_back(L(start,finish));
    }
    bool expected = exact_feasible(lines);
    for (int repeat = 0; repeat < 3; ++repeat) {
      if (halfplane_feasible_closed(lines) != expected) {
        cerr << "closed HPI mismatch at trial " << trial << '\n';
        for (cl line : lines)
          cerr << line.s.x << ' ' << line.s.y << ' '
               << line.t.x << ' ' << line.t.y << '\n';
        return 1;
      }
    }
  }
  // 大量重复约束：退化可行交集，不应退化为每次都扫描前缀。
  vector<L> many(200000, L({0,1},{1,1}));
  many.push_back(L({1,1},{0,1}));
  assert(halfplane_feasible_closed(many));
  cerr << "closed halfplane tests passed (60000 exact-oracle comparisons)\n";
}
