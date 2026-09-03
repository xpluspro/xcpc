// 点分树

#include <bits/stdc++.h>
using namespace std;
constexpr int K = 17;
constexpr int N = 1e5 + 5;
int n, m, fa[N], lg[N], val[N];
vector<int> e[N], C0[N], C1[N];
void add(vector<int> &c, int x, int v) {
  x++;
  while(x <= c.size()) c[x - 1] += v, x += x & -x;
}
int query(vector<int> &c, int x) {
  x = min(x + 1, (int) c.size()); // 注意 x 和 c.size() 取 min
  int s = 0;
  while(x) s += c[x - 1], x -= x & -x;
  return s;
}
int dn, dep[N], dfn[N], mi[K][N];
int get(int x, int y) {return dep[x] < dep[y] ? x : y;}
int lca(int x, int y) {
  if(x == y) return x;
  if((x = dfn[x]) > (y = dfn[y])) swap(x, y);
  int d = lg[y - x++];
  return get(mi[d][x], mi[d][y - (1 << d) + 1]);
}
int dis(int x, int y) {return dep[x] + dep[y] - 2 * dep[lca(x, y)];}
void dfs(int id, int ff) {
  dep[id] = dep[ff] + 1;
  mi[0][dfn[id] = ++dn] = ff;
  for(int it : e[id]) if(it != ff) dfs(it, id);
}
int sz[N], mx[N], vis[N], R, mxd;
void findroot(int id, int ff, int tot) {
  sz[id] = 1, mx[id] = 0;
  for(int it : e[id]) {
    if(it == ff || vis[it]) continue;
    findroot(it, id, tot);
    sz[id] += sz[it];
    mx[id] = max(mx[id], sz[it]);
  }
  mx[id] = max(mx[id], tot - sz[id]);
  if(mx[id] < mx[R]) R = id;
}
void getdep(int id, int ff, int anc) {
  mxd = max(mxd, dis(id, anc)), sz[id] = 1;
  for(int it : e[id]) if(!vis[it] && it != ff) getdep(it, id, anc), sz[id] += sz[it];
}
void divide(int id) {
  vis[id] = 1;
  mxd = 0, getdep(id, 0, id);
  int tmp = mxd + 1; // 注意这里要开 mxd + 1
  C0[id].resize(tmp);
  for(int it : e[id]) {
    if(vis[it]) continue;
    R = 0, findroot(it, id, sz[it]);
    C1[R].resize(tmp);
    fa[R] = id, divide(R);
  }
}
void add(int x, int v) {
  int u = x;
  while(u) {
    add(C0[u], dis(u, x), v);
    if(fa[u]) add(C1[u], dis(fa[u], x), v);
    u = fa[u];
  }
}
int query(int x, int k) {
  int u = x, ans = 0;
  while(u) {
    if(dis(u, x) <= k) ans += query(C0[u], k - dis(u, x));
    if(fa[u] && dis(fa[u], x) <= k) ans -= query(C1[u], k - dis(fa[u], x));
    u = fa[u];
  }
  return ans;
}
int main() {
  cin >> n >> m, mx[0] = N;
  for(int i = 1; i <= n; i++) cin >> val[i];
  for(int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  dfs(1, 0);
  for(int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
  for(int i = 1; i <= lg[n]; i++)
    for(int j = 1; j + (1 << i) - 1 <= n; j++)
      mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << i - 1)]);
  findroot(1, 0, n), divide(R);
  for(int i = 1; i <= n; i++) add(i, val[i]);
  for(int i = 1, las = 0; i <= m; i++) {
    int op, x, y;
    cin >> op >> x >> y, x ^= las, y ^= las;
    if(op == 0) cout << (las = query(x, y)) << "\n";
    else add(x, y - val[x]), val[x] = y;
  }
  return 0;
}