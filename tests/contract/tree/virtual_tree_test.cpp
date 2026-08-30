#include <gtest/gtest.h>
#define N 64
#include "prelude/common.hpp"

int n, id[N], dep[N], fa[N][8];
int s[N];
namespace vt {
vector<int> e[N];
void push(int u, int v) { e[u].push_back(v); }
}

int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = 7; k >= 0; --k)
    if (dep[u] - (1 << k) >= dep[v]) u = fa[u][k];
  if (u == v) return u;
  for (int k = 7; k >= 0; --k)
    if (fa[u][k] != fa[v][k]) u = fa[u][k], v = fa[v][k];
  return fa[u][0];
}

void dfs_id(int u, int p, int &c) {
  id[u] = ++c;
  dep[u] = dep[p] + 1;
  fa[u][0] = p;
  for (int k = 1; k < 8; ++k) fa[u][k] = fa[fa[u][k - 1]][k];
}

#include "src/sections/Tree/assets/tree/虚树.cpp"


TEST(VirtualTree, BuildsAndConnectsKeys) {
  n = 5;
  vector<vector<int>> g(6);
  g[1] = {2, 3};
  g[2] = {4};
  g[3] = {5};
  memset(fa, 0, sizeof fa);
  int c = 0;
  function<void(int, int)> dfs = [&](int u, int p) {
    dfs_id(u, p, c);
    for (int v : g[u])
      if (v != p) dfs(v, u);
  };
  dfs(1, 0);
  for (int i = 1; i <= n; ++i) vt::e[i].clear();
  build({1, 4, 5});
  int edges = 0;
  for (int i = 1; i <= n; ++i) edges += (int)vt::e[i].size();
  EXPECT_GE(edges, 2);
}
