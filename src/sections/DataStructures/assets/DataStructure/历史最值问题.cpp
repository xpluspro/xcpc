// 线段树维护历史最值问题

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int N = 1e5 + 5;
constexpr ll inf = 1e18;
ll n, m, a[N];
struct tag {
  ll ad, as, had, has;
  tag operator + (const tag &z) const { // 标记合并
    tag res;
    if(as == -inf && z.as == -inf) {
      res.ad = ad + z.ad;
      res.as = -inf;
      res.had = max(had, ad + z.had);
      res.has = -inf;
    }
    if(as == -inf && z.as != -inf) {
      res.ad = 0;
      res.as = z.as;
      res.had = max(had, ad + z.had);
      res.has = z.has;
    }
    if(as != -inf && z.as == -inf) {
      res.ad = 0;
      res.as = as + z.ad;
      res.had = had;
      res.has = max(has, as + z.had);
    }
    if(as != -inf && z.as != -inf) {
      res.ad = 0;
      res.as = z.as;
      res.had = had;
      res.has = max(has, max(as + z.had, z.has));
    }
    return res;
  }
} laz[N << 2];
struct dat {
  ll his, mx;
  dat operator + (const dat &z) const { // 信息合并
    return {max(his, z.his), max(mx, z.mx)};
  }
  dat operator + (const tag &z) const { // 标记作用在信息上
    return {max(his, max(mx + z.had, z.has)), z.as == -inf ? mx + z.ad : z.as};
  }
} val[N << 2];
void build(int l, int r, int x) {
  if(l == r) {
    val[x] = {a[l], a[l]};
    return;
  }
  int m = l + r >> 1;
  build(l, m, x << 1);
  build(m + 1, r, x << 1 | 1);
  val[x] = val[x << 1] + val[x << 1 | 1];
  laz[x] = {0, -inf, 0, -inf};
}
void adtag(int x, tag v) {
  val[x] = val[x] + v;
  laz[x] = laz[x] + v;
}
void down(int x) {
  adtag(x << 1, laz[x]);
  adtag(x << 1 | 1, laz[x]);
  laz[x] = {0, -inf, 0, -inf}; // 注意清空标记
}
void modify(int l, int r, int ql, int qr, int x, tag v) {
  if(ql <= l && r <= qr) return adtag(x, v);
  int m = l + r >> 1;
  down(x);
  if(ql <= m) modify(l, m, ql, qr, x << 1, v);
  if(m < qr) modify(m + 1, r, ql, qr, x << 1 | 1, v);
  val[x] = val[x << 1] + val[x << 1 | 1];
}
dat query(int l, int r, int ql, int qr, int x) {
  if(ql <= l && r <= qr) return val[x];
  int m = l + r >> 1;
  down(x);
  dat res = {-inf, -inf};
  if(ql <= m) res = res + query(l, m, ql, qr, x << 1);
  if(m < qr) res = res + query(m + 1, r, ql, qr, x << 1 | 1);
  return res;
}
int main() {
  cin >> n;
  for(int i = 1; i <= n; i++) cin >> a[i];
  build(1, n, 1);
  cin >> m;
  for(int i = 1; i <= m; i++) {
    char op;
    cin >> op;
    int x, y, z;
    if(op == 'A') cin >> x >> y, cout << query(1, n, x, y, 1).his << "\n";
    if(op == 'Q') cin >> x >> y, cout << query(1, n, x, y, 1).mx << "\n";
    if(op == 'P') cin >> x >> y >> z, modify(1, n, x, y, 1, {z, -inf, max(0, z), -inf});
    if(op == 'C') cin >> x >> y >> z, modify(1, n, x, y, 1, {0, z, 0, z});
  }
  return 0;
}