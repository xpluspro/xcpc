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

// 仅供旧线数据测试使用；正式 API 只有 vector<HP>。
bool check_lines(const vector<L>& lines, LD tolerance = 0) {
  vector<HP> h(lines.begin(), lines.end());
  for (auto& row : h) if (row.n.len2()) row.c -= tolerance * hypotl(row.n.x, row.n.y);
  return hp_feasible(h);
}

int main() {
  const LD infinity = HINF;
  assert(hp_feasible({}, {0,0}));
  assert(!hp_feasible({}, {0,0,true,false}));
  assert(!hp_feasible({}, {0,0,false,true}));
  assert(!hp_feasible({}, {1,0}));
  assert(!hp_feasible({}, {infinity,infinity}));
  assert(!hp_feasible({}, {-infinity,-infinity}));
  assert(hp_feasible({{{1,0},1}}, {0,1,true,false}));
  assert(!hp_feasible({{{1,0},1}}, {0,1,false,true}));
  assert(hp_feasible({{{-1,0},-1}}, {1,2,false,true}));
  assert(!hp_feasible({{{-1,0},-1}}, {1,2,true,false}));
  vector<HP> decreasing{{{0,1},0},{{1,-1},0}}; // F(x)=-x
  assert(!hp_feasible(decreasing,x_lt(0)));
  assert(hp_feasible(decreasing,{-infinity,0}));
  vector<HP> increasing{{{0,1},0},{{-1,-1},0}};
  assert(!hp_feasible(increasing,x_gt(0)));
  assert(hp_feasible(increasing,{0,infinity}));
  vector<HP> flat{{{0,1},0},{{0,-1},0}};
  assert(hp_feasible(flat,{-1,0,true,true}));
  vector<HP> tiny{{{0,1},0},{{1,-1},-1e-30L}};
  assert(hp_feasible(tiny,x_lt(0)));
  tiny.back().c=1e-30L;
  assert(!hp_feasible(tiny,x_lt(0)));
  // 严格可行区间 (-delta,0)，不能用 v < -e 将其吞掉。
  LD delta=ldexpl(1.0L,-60);
  assert(hp_feasible({{{0,1},1},{{1,-1},-1-delta}},x_lt(0)));
  assert(!hp_feasible({{{0,1},1},{{1,-1},-1}},x_lt(0)));
  vector<HP> cusp{{{1,1},0},{{-1,1},0},{{0,-1},0}};
  assert(!hp_feasible(cusp,x_lt(0)));
  assert(!hp_feasible(cusp,x_gt(0)));
  assert(hp_feasible(cusp,{-1,1,true,true}));
  rnd.seed(20260910);
  // 用户回归：2 1 19 / 10 7 7，含非负 A、B，解退化到线段。
  vector<L> regression = {{{0,1},{0,0}}, {{0,0},{1,0}}};
  for (auto [x,c,d] : vector<array<LD,3>>{{2,1,19},{10,7,7}}) {
    regression.push_back(L({0,c/x},{1,c/x-x}));
    regression.push_back(L({1,d/x-x},{0,d/x}));
  }
  for (int seed = 0; seed < 30; ++seed) {
    shuffle(regression.begin(), regression.end(), rnd);
    assert(check_lines(regression));
  }
  vector<HP> rows = {
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
    row = HP(row.n * 1e20L, row.c * 1e20L);
  }
  // 从原始系数分别缩放后再构造，覆盖系数缩放及非轴对齐单点。
  for (LD scale : {1e-20L, 1.0L, 1e20L}) {
    vector<HP> scaled_point = {
      {{3*scale,4*scale},5*scale}, {{-3,-4},-5},
      {{4,-3},0}, {{-4*scale,3*scale},0}
    }; // 唯一解 (0.6, 0.8)
    auto scaled_empty = scaled_point;
    scaled_empty.push_back({{1,0},0.61L});
    for (int seed = 0; seed < 30; ++seed) {
      shuffle(scaled_point.begin(), scaled_point.end(), rnd);
      assert(hp_feasible(scaled_point));
      assert(!hp_feasible(scaled_empty));
    }
  }
  for (int seed = 0; seed < 30; ++seed) {
    shuffle(rows.begin(), rows.end(), rnd);
    assert(hp_feasible(rows));
    assert(hp_feasible(point));
    assert(!hp_feasible(impossible));
    assert(!hp_feasible(scaled_impossible));
  }
  // 原点附近的真实间隙，不允许被固定绝对 EPS 吞掉。
  assert(!hp_feasible({{{1,0},1e-25L},{{-1,0},0}}));
  assert(!hp_feasible({{{0,1},1e-25L},{{0,-1},0}}));
  // 只有远处才有可行点，且不用人为大框。
  assert(hp_feasible({{{1e-30L,0},1},{{0,1},0}}));
  assert(hp_feasible({{{0,1},1},{{1e-30L,-1},0}}));
  // 固定 x，恰好落在两包络折点；以及唯一可行位置在区间右端点。
  assert(hp_feasible({{{1,0},0},{{-1,0},0},
    {{-1,1},0},{{1,1},0},{{0,-1},0}}));
  assert(hp_feasible({{{0,1},1},{{1,-1},0},{{-1,0},-1}}));
  assert(!hp_feasible({{{0,1},1},{{1,-1},0},{{-1,0},0}}));
  assert(hp_feasible({}));
  assert(hp_feasible({{{0,0},0},{{0,0},-1}}));
  assert(!hp_feasible({{{0,0},1}}));
  // 方向不经过端点相减：很大的截距也保留原始平行关系。
  assert(!hp_feasible({
    {{10,1},1e18L}, {{-10,-1},-1e18L+100}}));
  // 非零的小夹角不能简单当平行：交点在远处。
  assert(hp_feasible({
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
      assert(check_lines(lines) == expected);
      assert(check_lines(lines, eps) == expected);
    }

  // 容差为距离，与有向边长度无关；零容差不主动放宽边界。
  vector<L> tiny_gap = {{{0,0},{1,0}},{{1,-eps},{0,-eps}}};
  assert(check_lines(tiny_gap, eps));
  assert(!check_lines(tiny_gap));
  tiny_gap[0].t = P(1e6L,0);
  assert(check_lines(tiny_gap, eps));
  vector<L> larger_gap = {{{0,0},{1,0}},{{1,-4*eps},{0,-4*eps}}};
  assert(!check_lines(larger_gap, eps));

  // P3222 形状：原始整数约束 x^2*A+x*B 在 [y1,y2]，x 可接近 1e9。
  // 有界候选两两求交，以 __int128 精确验证；取值使中间结果小于 1e38。
  mt19937_64 trajectory_rng(20260910);
  using I = __int128_t;
  struct Row { I a,b,c; };
  for (int trial=0;trial<3000;++trial) {
    vector<Row> exact{{1,0,-10000},{-1,0,0},{0,1,0},{0,-1,-10000000000LL}};
    vector<HP> h{{{1,0},-10000},{{-1,0},0},{{0,1},0},{{0,-1},-1e10L}};
    int n=1+trajectory_rng()%8;
    for (int i=0;i<n;++i) {
      long long x=trial%2 ? 1000000000-trajectory_rng()%100 : 1+trajectory_rng()%1000000000;
      long long y1=1+trajectory_rng()%1000000, y2=y1+trajectory_rng()%1000000;
      if (trial%3==0) y2=y1;
      exact.push_back({(I)x*x,x,y1}); exact.push_back({-(I)x*x,-x,-y2});
      h.emplace_back(P{(LD)x*x,(LD)x},(LD)y1);
      h.emplace_back(P{-(LD)x*x,-(LD)x},-(LD)y2);
    }
    bool expected=false;
    for (const Row& a:exact) for (const Row& b:exact) {
      I d=a.a*b.b-a.b*b.a;
      if (!d) continue;
      I x=a.c*b.b-a.b*b.c, y=a.a*b.c-a.c*b.a;
      if (d<0) d=-d,x=-x,y=-y;
      bool valid=true;
      for (const Row& row:exact) if(row.a*x+row.b*y<row.c*d) { valid=false; break; }
      expected|=valid;
    }
    if (hp_feasible(h)!=expected) {
      cerr<<"trajectory mismatch "<<trial<<" expected "<<expected<<'\n';
      return 1;
    }
  }

  // 已知可行的极近方向等式组：共同经过 (-1, 1e9+100)。
  vector<HP> pencil;
  for (long long x=999999900;x<=1000000000;x+=5) {
    LD intercept=1000000100LL-x;
    pencil.emplace_back(P{(LD)x,1},intercept);
    pencil.emplace_back(P{-(LD)x,-1},-intercept);
  }
  assert(hp_feasible(pencil));
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
      shuffle(lines.begin(), lines.end(), rnd);
      if (check_lines(lines) != expected) {
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
  assert(check_lines(many));
  // 两侧各有大量不同斜率的包络，交为线段；再加入平行矛盾。
  vector<HP> envelope_rows;
  for (int k=-10000;k<=10000;++k) {
    envelope_rows.push_back({{(LD)-2*k,1},-(LD)k*k});
    envelope_rows.push_back({{(LD)-2*k,-1},-(LD)k*k});
  }
  assert(hp_feasible(envelope_rows));
  envelope_rows.push_back({{0,1},1});
  assert(!hp_feasible(envelope_rows));
  cerr << "closed halfplane tests passed (60000 exact-oracle + 3000 trajectory comparisons)\n";
}
