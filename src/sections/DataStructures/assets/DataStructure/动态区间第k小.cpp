// 动态区间第 k 小

#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e5 + 5;
constexpr int K = N << 9;
int n, m, node, a[N], R[N], ls[K], rs[K], val[K];
void modify(int l, int r, int p, int &x, int v) {
  if(!x) x = ++node;
  val[x] += v;
  if(l == r) return;
  int m = l + r >> 1;
  if(p <= m) modify(l, m, p, ls[x], v);
  else modify(m + 1, r, p, rs[x], v);
}
vector<int> Add, Sub;
int query(int l, int r, int k) {
  if(l == r) return l;
  int m = l + r >> 1, v = 0;
  for(int it : Add) v += val[ls[it]];
  for(int it : Sub) v -= val[ls[it]];
  for(int &it : Add) it = k <= v ? ls[it] : rs[it];
  for(int &it : Sub) it = k <= v ? ls[it] : rs[it];
  if(k <= v) return query(l, m, k);
  return query(m + 1, r, k - v);
}
void add(int x, int y, int v) {
  while(x <= n) modify(0, 1e9, y, R[x], v), x += x & -x;
}
int main() {
  cin >> n >> m;
  for(int i = 1; i <= n; i++) cin >> a[i], add(i, a[i], 1);
  for(int i = 1, l, r, k; i <= m; i++) {
    char op;
    cin >> op >> l >> r;
    if(op == 'C') add(l, a[l], -1), add(l, a[l] = r, 1);
    else {
      cin >> k, Add.clear(), Sub.clear();
      int x = r;
      while(x) Add.push_back(R[x]), x -= x & -x;
      x = l - 1;
      while(x) Sub.push_back(R[x]), x -= x & -x;
      cout << query(0, 1e9, k) << "\n";
    }
  }
  return 0;
}