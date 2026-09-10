// 有向直线左侧为合法区域；普通点判定包含边界（沿用 turn 的 eps）。
bool on_left(cl l, cp p) { return turn(l.s, l.t, p) >= 0; }
// HPI deque 内部专用：必须严格在左侧，不能改成 >= 0。
bool hpi_inside(cl a, cl b, cl c) {
  return turn(a.s, a.t, ll_intersection(b, c)) > 0; }
bool is_para(cl a, cl b){return !sgn((a.t-a.s) ^ (b.t-b.s));}
bool cmp(cl a, cl b) {
  int sign = half(a.t - a.s) - half(b.t - b.s);
  int dir = sgn((a.t - a.s) ^ (b.t - b.s));
  // 同方向时，更严格的半平面排在前面。
  if (!dir && !sign) return turn(a.s, a.t, b.t) < 0;
  else return sign ? sign > 0 : dir > 0; }
// 输入必须已按 cmp 排序（有序序列的子序列仍可直接使用），O(n)。
// 左侧合法，只返回正面积的有界多边形；无界问题必须预先加足够大的框。
// 零面积交（线段/点）不支持，视为空；判定沿用 turn 的 eps。
vp hpi_sorted(const vector<L>& h) {
  vector<L> q(h.size()); int l = 0, r = -1;
  for(auto &i : h) {
   while (l < r && !hpi_inside(i, q[r - 1], q[r])) --r;
   while (l < r && !hpi_inside(i, q[l], q[l + 1])) ++l;
   if (l <= r && is_para(i, q[r])) continue;
   q[++r] = i; }
  while (r - l > 1 && !hpi_inside(q[l], q[r - 1], q[r])) --r;
  while (r - l > 1 && !hpi_inside(q[r], q[l], q[l + 1])) ++l;
  if(r - l < 2) return {};
  vp ret(r - l + 1);
  for(int i = l; i <= r; i++) 
	ret[i - l] = ll_intersection(q[i], q[i == r ? l : i + 1]);
  return ret; }
// 空集会在队列里留下一个开区域；开区域会被判定为空集。
// 左侧合法；返回正面积有界 HPI，零面积交视为空。O(n log n)。
// 无界问题必须预先加 bounding box；hpi_inside 必须保持 > 0。
vp hpi_polygon(vector<L> h) {
  sort(h.begin(), h.end(), cmp);
  return hpi_sorted(h); }
// CCW，统一取左侧，方向向量长度为 1；须在排序之前添加。
// 要求 xmin < xmax、ymin < ymax，且坐标精度足以表示加减 1。
void add_box(vector<L>& h, LD xmin, LD xmax, LD ymin, LD ymax) {
  h.push_back({{xmin, ymin}, {xmin + 1, ymin}}); // y >= ymin
  h.push_back({{xmax, ymin}, {xmax, ymin + 1}}); // x <= xmax
  h.push_back({{xmax, ymax}, {xmax - 1, ymax}}); // y <= ymax
  h.push_back({{xmin, ymax}, {xmin, ymax - 1}}); // x >= xmin
}
// 闭半平面可行性（允许线段/点）需独立算法，不能靠改成 >= 0 扩展本函数。
