//线段树套线段树维护矩形取最大值矩形求最大值

#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e3 + 5;
int n, D, S;
namespace ST {
  int node, val[N << 11], laz[N << 11], ls[N << 11], rs[N << 11];
  void modify(int l, int r, int ql, int qr, int &x, int v) {
    if(!x) x = ++node;
    laz[x] = max(laz[x], v);
    if(ql <= l && r <= qr) return val[x] = max(val[x], v), void();
    int m = l + r >> 1;
    if(ql <= m) modify(l, m, ql, qr, ls[x], v);
    if(m < qr) modify(m + 1, r, ql, qr, rs[x], v);
  }
  int query(int l, int r, int ql, int qr, int x) {
    if(!x) return 0;
    if(ql <= l && r <= qr) return max(laz[x], val[x]);
    int m = l + r >> 1, ans = val[x];
    if(ql <= m) ans = max(ans, query(l, m, ql, qr, ls[x]));
    if(m < qr) ans = max(ans, query(m + 1, r, ql, qr, rs[x]));
    return ans;
  }
}
int laz[N << 2], val[N << 2];
void modify(int l, int r, int ql, int qr, int u, int d, int x, int v) {
  ST::modify(1, S, u, d, laz[x], v);
  if(ql <= l && r <= qr) return ST::modify(1, S, u, d, val[x], v), void();
  int m = l + r >> 1;
  if(ql <= m) modify(l, m, ql, qr, u, d, x << 1, v);
  if(m < qr) modify(m + 1, r, ql, qr, u, d, x << 1 | 1, v);
}
int query(int l, int r, int ql, int qr, int u, int d, int x) {
  int ans = ST::query(1, S, u, d, val[x]);
  if(ql <= l && r <= qr) return max(ans, ST::query(1, S, u, d, laz[x]));
  int m = l + r >> 1;
  if(ql <= m) ans = max(ans, query(l, m, ql, qr, u, d, x << 1));
  if(m < qr) ans = max(ans, query(m + 1, r, ql, qr, u, d, x << 1 | 1));
  return ans;
}
int main() {
  cin >> D >> S >> n;
  for(int i = 1; i <= n; i++) {
    int d, s, w, x, y; cin >> d >> s >> w >> x >> y;
    int ht = query(1, D, x + 1, x + d, y + 1, y + s, 1);
    modify(1, D, x + 1, x + d, y + 1, y + s, 1, ht + w);
  }
  cout << query(1, D, 1, D, 1, S, 1) << endl;
  return 0;
}