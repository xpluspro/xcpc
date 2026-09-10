// 左侧为合法闭半平面；支持无界、线段、单点和空约束，不需要加框。
// 随机增量：维护离原点最近的可行点；违反新约束时在其边界上解一维区间。
// 期望 O(n)，最坏 O(n^2)，空间 O(n)。不要用于每次都要求线性上界的场景。
// tolerance >= 0 是距离容差：默认 0；显式传正数才将边界向外放宽。
// tolerance = 0 不主动放宽，但浮点运算仍有舍入误差；精确判定需精确算术。
// 输入及中间运算须为有限数（区间的无穷端点除外）。
struct ClosedHalfplane { P normal; LD bound; }; // normal * point >= bound
// 已知不等式系数时优先使用此接口，避免由两点相减重建方向。
// 例如 x*A+B >= c/x 可直接写 {{x,1},c/x}，x > 0 时也可写 {{x*x,x},c}。
bool halfplane_feasible_closed_coefficients(vector<ClosedHalfplane> constraints,
    LD tolerance = 0) {
  for (auto& row : constraints) {
    LD length = hypotl(row.normal.x, row.normal.y);
    if (length == 0 && row.bound > 0) return false;
    row.bound -= tolerance * length;
  }
  shuffle(constraints.begin(), constraints.end(), rnd);
  // 只补偿点积舍入量级，避免同一边界的不同倍数因舍入被误判为矛盾。
  auto violated = [](const ClosedHalfplane& row, cp point) {
    LD x = row.normal.x * point.x, y = row.normal.y * point.y;
    LD error = 8 * numeric_limits<LD>::epsilon()
        * (fabsl(x) + fabsl(y) + fabsl(row.bound));
    return row.bound - (x + y) > error;
  };
  P feasible;
  const LD infinity = numeric_limits<LD>::infinity();
  for (int i = 0; i < (int)constraints.size(); ++i) {
    const auto& current = constraints[i];
    if (!violated(current, feasible)) continue;
    // 新的最近点必在这条边界上：base + direction * t。
    P base = fabsl(current.normal.x) >= fabsl(current.normal.y)
        ? P(current.bound / current.normal.x, 0)
        : P(0, current.bound / current.normal.y);
    P direction = current.normal.rot90();
    // t 随方向长度反比缩放，误差下限也必须相应缩放。
    LD parameter_unit = 1 / hypotl(direction.x, direction.y);
    LD lower = -infinity, upper = infinity;
    for (int j = 0; j < i; ++j) {
      const auto& previous = constraints[j];
      LD coefficient = previous.normal * direction;
      LD residual = previous.bound - previous.normal * base;
      // 不能用 sgn(coefficient)：极小的非零夹角仍可能在很远处相交。
      if (coefficient > 0) lower = max(lower, residual / coefficient);
      else if (coefficient < 0) upper = min(upper, residual / coefficient);
      else if (violated(previous, base)) return false;
      if (lower > upper) {
        // 先排除无穷端点，防止误差尺度变成 inf 而吞掉空区间。
        if (!isfinite(lower) || !isfinite(upper)) return false;
        LD error = 64 * numeric_limits<LD>::epsilon()
            * max({parameter_unit, fabsl(lower), fabsl(upper)});
        if (lower - upper > error) return false;
        // clamp 要求 lower <= upper；舍入量级的倒置按单点处理。
        lower = upper = lower / 2 + upper / 2;
      }
    }
    LD nearest = -(base * direction) / direction.len2();
    feasible = base + direction * clamp(nearest, lower, upper);
  }
  return true;
}

// 两点接口只能处理实际存储的方向，无法恢复端点构造时丢失的平行关系。
// 不要用固定角度 eps 强行判平行，否则会丢掉远处的真实可行交点。
// 零长度有向线按 turn == 0 视为无约束。
bool halfplane_feasible_closed(const vector<L>& h, LD tolerance = 0) {
  vector<ClosedHalfplane> constraints;
  constraints.reserve(h.size());
  for (cl line : h) {
    P direction = line.t - line.s;
    if (direction.x == 0 && direction.y == 0) continue;
    P normal = direction.rot90();
    constraints.push_back({normal, normal * line.s});
  }
  return halfplane_feasible_closed_coefficients(move(constraints), tolerance);
}
