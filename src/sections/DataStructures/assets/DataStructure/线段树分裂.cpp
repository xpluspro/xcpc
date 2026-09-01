// 线段树分裂
void split(int x, int &y, int k) {
  if(!x) return y = 0, void();
  y = ++node;
  if(k < val[ls[x]]) swap(rs[x], rs[y]), split(ls[x], ls[y], k);
  else if(k == val[ls[x]]) swap(rs[x], rs[y]);
  else split(rs[x], rs[y], k - val[ls[x]]);
  val[y] = val[x] - k, val[x] = k;
}
void split(int l, int r, int x, int &y, int k) {
  if(!x || k == r) return y = 0, void();
  y = ++node;
  int m = l + r >> 1;
  if(k <= m) swap(rs[x], rs[y]), split(l, m, ls[x], ls[y], k);
  else split(m + 1, r, rs[x], rs[y], k);
  val[x] = val[ls[x]] + val[rs[x]];
  val[y] = val[ls[y]] + val[rs[y]];
}