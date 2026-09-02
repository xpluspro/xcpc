// 动态逆序对

#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e5 + 5;
constexpr int K = N << 9;
int n, m, node, a[N], rev[N], R[N], ls[K], rs[K], val[K];
long long ans;
void modify(int l, int r, int p, int &x, int v) {
  if(!x) x = ++node;
  val[x] += v;
  if(l == r) return;
  int m = l + r >> 1;
  if(p <= m) modify(l, m, p, ls[x], v);
  else modify(m + 1, r, p, rs[x], v);
}
int query(int l, int r, int ql, int qr, int x) {
  if(ql <= l && r <= qr) return val[x];
  int m = l + r >> 1, ans = 0;
  if(ql <= m) ans = query(l, m, ql, qr, ls[x]);
  if(m < qr) ans += query(m + 1, r, ql, qr, rs[x]);
  return ans;
}
void add(int x, int y, int v) {while(x <= n) modify(1, n, y, R[x], v), x += x & -x;}
int query(int x, int yd, int yu) {int s = 0; while(x) s += query(1, n, yd, yu, R[x]), x -= x & -x; return s;}
int query(int xl, int xr, int yd, int yu) {return yd > yu ? 0 : query(xr, yd, yu) - query(xl - 1, yd, yu);}
int main() {
  cin >> n >> m;
  for(int i = 1; i <= n; i++) cin >> a[i], rev[a[i]] = i, add(i, a[i], 1);
  for(int i = 2; i <= n; i++) ans += query(1, i - 1, a[i] + 1, n);
  for(int i = 1, p; i <= m; i++) {
    cin >> p, p = rev[p], cout << ans << "\n";
    ans -= query(1, p - 1, a[p] + 1, n) + query(p + 1, n, 1, a[p] - 1)；
    add(p, a[p], -1);
  }
  return 0;
}