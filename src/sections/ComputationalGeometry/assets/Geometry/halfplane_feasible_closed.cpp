// n * p >= b；按题目原始表示构造：系数用 HP({a,b},c)，两点用 HP(L)。
// 已知系数不要先造浮点端点再转回来，以免主动丢失平行关系。
struct HP {
  P n; LD b;
  HP(P normal = {}, LD bound = 0) : n(normal), b(bound) {
    LD d = hypotl(n.x, n.y);
    if (d) n = n / d, b /= d;
  }
  HP(cl l) : HP((l.t - l.s).rot90(), (l.t - l.s).rot90() * l.s) {} // 左侧合法
};

bool hp_bad(const HP& h, cp p) {
  LD x = h.n.x * p.x, y = h.n.y * p.y;
  LD e = 16 * numeric_limits<LD>::epsilon()
      * (1 + fabsl(x) + fabsl(y) + fabsl(h.b));
  return h.b - (x + y) > e;
}

// 闭半平面交是否非空：支持无界、线段、单点及空约束。
// 随机增量，期望 O(n)，最坏 O(n^2)，空间 O(n)。
// 仅补偿浮点舍入，非精确判定；输入及中间运算须有限（区间端点除外）。
// 若需距离容差，调用前对非零法向量的 HP 手动 b -= tolerance。
bool hp_feasible(vector<HP> a) {
  shuffle(a.begin(), a.end(), rnd);
  const LD inf = numeric_limits<LD>::infinity();
  P p{};
  for (int i = 0; i < (int)a.size(); ++i) {
    if (!a[i].n.len2()) {
      if (a[i].b > 0) return false;
      continue;
    }
    if (!hp_bad(a[i], p)) continue;
    P o = a[i].n * a[i].b, v = a[i].n.rot90();
    LD l = -inf, r = inf;
    for (int j = 0; j < i; ++j) {
      LD k = a[j].n * v, c = a[j].b - a[j].n * o;
      // 不用固定角度 eps 判平行：小夹角可能在远处有解。
      if (k > 0) l = max(l, c / k);
      else if (k < 0) r = min(r, c / k);
      else if (hp_bad(a[j], o)) return false;
    }
    if (l > r) {
      if (!isfinite(l) || !isfinite(r)) return false;
      LD e = 64 * numeric_limits<LD>::epsilon()
          * max({1.0L, fabsl(l), fabsl(r)});
      if (l - r > e) return false;
      l = r = l / 2 + r / 2;
    }
    p = o + v * clamp(0.0L, l, r);
  }
  return true;
}
