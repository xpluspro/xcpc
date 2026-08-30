#include <gtest/gtest.h>
#define N 64
#define M 256
#include "prelude/common.hpp"

struct Edge {
  int to, next, w;
} e[M];
int head[N], dep[N], cur[N], s, t, n, tot;

void add_flow(int u, int v, int w) {
  e[++tot] = {v, head[u], w};
  head[u] = tot;
  e[++tot] = {u, head[v], 0};
  head[v] = tot;
}

#include "src/sections/GraphTheory/assets/graph/Dinic.cpp"


static int maxflow() {
  int f = 0, x;
  while (bfs())
    while ((x = dfs(s, INF))) f += x;
  return f;
}

TEST(Dinic, SmallNetworks) {
  auto run = [](int nv, int src, int sink, vector<tuple<int, int, int>> eds) {
    memset(head, 0, sizeof head);
    tot = 1; // so i^1 works if we started at 0? snippet uses i^1, tot starts 1 then ++ makes 2,3 pairs: 2^1=3, 3^1=2. Need tot start at 1 so first pair is 2 and 3.
    n = nv;
    s = src;
    t = sink;
    for (auto [u, v, w] : eds) add_flow(u, v, w);
    return maxflow();
  };
  // convention s=n+1, t=s+1 used in comment; we still set s,t explicitly
  EXPECT_EQ(run(2, 1, 2, {{1, 2, 5}}), 5);
  EXPECT_EQ(run(4, 1, 4, {{1, 2, 1}, {1, 3, 1}, {2, 4, 1}, {3, 4, 1}}), 2);
  EXPECT_EQ(run(3, 1, 3, {{1, 2, 10}}), 0);
  EXPECT_EQ(run(2, 1, 2, {}), 0);
}
