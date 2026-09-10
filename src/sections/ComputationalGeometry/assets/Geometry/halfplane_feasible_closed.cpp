// 左侧为合法闭半平面；支持无界、线段、单点和空约束，不需要加框。
// 确定性最坏 O(n log n) 时间、O(n) 空间：排序建包络，再线性归并区间。
// tolerance >= 0 是距离容差：将每条边界向外放宽 tolerance 后判定。
// tolerance = 0 不主动放宽，但浮点运算仍有舍入误差；精确判定需精确算术。
// 零长度有向线按 turn == 0 视为无约束；其余输入及中间运算须为有限数。
bool halfplane_feasible_closed(const vector<L>& h, LD tolerance = eps) {
  const LD infinity = numeric_limits<LD>::infinity();
  struct Affine { LD slope, intercept, start; };
  vector<Affine> lower, negative_upper;
  LD xmin = -infinity, xmax = infinity;
  for (cl line : h) {
    P d = line.t - line.s;
    // 不使用 sgn：极小的非零方向分量仍可能决定远处的可行区域。
    if (d.x == 0) {
      if (d.y > 0) xmax = min(xmax, line.s.x + tolerance);
      if (d.y < 0) xmin = max(xmin, line.s.x - tolerance);
      continue;
    }
    // d.x * y - d.y * x >= bound。
    LD bound = fmal(d.x, line.s.y, -d.y * line.s.x)
        - tolerance * hypotl(d.x, d.y);
    if (d.x > 0) lower.push_back({d.y / d.x, bound / d.x, 0});
    else negative_upper.push_back({-d.y / d.x, -bound / d.x, 0});
  }
  if (xmin > xmax) return false;
  // 只有一侧 y 约束时，可在任意合法 x 处取足够大/小的 y。
  if (lower.empty() || negative_upper.empty()) return true;
  auto envelope = [infinity](vector<Affine> lines) {
    // 普通浮点大小比较，不用 eps，确保排序比较器满足严格弱序。
    sort(lines.begin(), lines.end(), [](const Affine& a, const Affine& b) {
      return a.slope != b.slope ? a.slope < b.slope : a.intercept > b.intercept;
    });
    vector<Affine> hull;
    for (auto line : lines) {
      if (!hull.empty() && line.slope == hull.back().slope) continue;
      line.start = -infinity;
      while (!hull.empty()) {
        line.start = (hull.back().intercept - line.intercept)
            / (line.slope - hull.back().slope);
        if (line.start > hull.back().start) break;
        hull.pop_back();
      }
      if (hull.empty()) line.start = -infinity;
      hull.push_back(line);
    }
    return hull;
  };
  lower = envelope(move(lower));
  negative_upper = envelope(move(negative_upper));
  // 存在 y 等价于 max(lower) + max(-upper) <= 0。
  // 归并两个包络的断点，每段上只需检查一个一次函数的最小值。
  size_t i = 0, j = 0;
  LD left = xmin;
  while (i + 1 < lower.size() && lower[i + 1].start <= left) ++i;
  while (j + 1 < negative_upper.size() && negative_upper[j + 1].start <= left) ++j;
  while (true) {
    LD next_lower = i + 1 < lower.size() ? lower[i + 1].start : infinity;
    LD next_upper = j + 1 < negative_upper.size() ? negative_upper[j + 1].start : infinity;
    LD right = min({xmax, next_lower, next_upper});
    const auto& a = lower[i];
    const auto& b = negative_upper[j];
    LD slope = a.slope + b.slope, intercept = a.intercept + b.intercept;
    // 无穷端点只用于判断下降方向，不对无穷坐标求值。
    if ((slope > 0 && left == -infinity) || (slope < 0 && right == infinity))
      return true;
    LD x = slope > 0 ? left : (slope < 0 ? right : clamp(0.0L, left, right));
    LD value = fmal(slope, x, intercept);
    // 补偿包络端点评估的浮点舍入；不把小斜率当作零。
    LD error = 8 * numeric_limits<LD>::epsilon()
        * (fabsl(a.slope * x) + fabsl(b.slope * x)
           + fabsl(a.intercept) + fabsl(b.intercept));
    if (value <= error) return true;
    if (right == xmax) return false;
    if (right == next_lower) ++i;
    if (right == next_upper) ++j;
    left = right;
  }
}
