// 点分治

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
#define fi first
#define se second
constexpr int N = 1e4 + 5;
int n, q, k[N], ans[N];
int R, vis[N], mx[N], sz[N];
vector<pii> e[N];
void findroot(int id, int ff, int tot) {
  sz[id] = 1, mx[id] = 0;
  for(pii _ : e[id]) {
    int it = _.fi;
    if(it == ff || vis[it]) continue;
    findroot(it, id, tot);
    sz[id] += sz[it];
    mx[id] = max(mx[id], sz[it]);
  }
  mx[id] = max(mx[id], tot - sz[id]);
  if(mx[id] < mx[R]) R = id;
}
vector<pii> info;
void getinfo(int id, int ff, int anc, int d) {
  info.push_back({d, anc});
  for(pii _ : e[id]) {
    int it = _.fi;
    if(it == ff || vis[it]) continue;
    getinfo(it, id, anc, d + _.se);
  }
}
void divide(int id) {
  vis[id] = 1;
  for(pii _ : e[id]) {
    int it = _.fi;
    if(vis[it]) continue;
    getinfo(it, id, it, _.se);
  }
  info.push_back({0, id});
  sort(info.begin(), info.end());
  for(int i = 1; i <= q; i++) {
    int l = 0, r = (int) info.size() - 1;
    while(l < r && !ans[i]) {
      if(info[l].fi + info[r].fi > k[i]) r--;
      else if(info[l].fi + info[r].fi < k[i]) l++;
      else {
        ans[i] = info[l].se != info[r].se;
        if(info[l].fi == info[l + 1].fi) l++;
        else r--;
      }
    }
  }
  info.clear();
  for(pii _ : e[id]) {
    int it = _.fi;
    if(vis[it]) continue;
    R = 0, findroot(it, id, sz[it]);
    divide(R);
  }
}
int main() {
  cin >> n >> q;
  for(int i = 1; i < n; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    e[u].push_back({v, w});
    e[v].push_back({u, w});
  }
  for(int i = 1; i <= q; i++) cin >> k[i];
  mx[0] = N; // 不要忘记将 mx[0] 设为最大值
  findroot(1, 0, n), divide(R);
  for(int i = 1; i <= q; i++) puts(ans[i] ? "AYE" : "NAY");
  return 0;
}