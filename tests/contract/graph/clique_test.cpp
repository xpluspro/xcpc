#include <gtest/gtest.h>
#include "prelude/common.hpp"

using ull = unsigned long long;
vector<ull> sol;
#define pb push_back

ull E[64];
ll ans;
int n;

#define bit(i) (1ULL << (i))
void dfs(ull P, ull X, ull R) {
  if (!P && !X) {
    ++ans;
    sol.pb(R);
    return;
  }
  ull Q = P & ~E[__builtin_ctzll(P | X)];
  for (int i; i = __builtin_ctzll(Q), Q; Q &= ~bit(i)) {
    dfs(P & E[i], X & E[i], R | bit(i));
    P &= ~bit(i), X |= bit(i);
  }
}

static int count_cliques() {
  for (int i = 0; i < n; ++i) assert((E[i] & bit(i)) == 0);
  ans = 0;
  sol.clear();
  dfs(n == 64 ? ~0ULL : bit(n) - 1, 0, 0);
  return (int)ans;
}


static int brute_cliques(int nv, const vector<ull> &adj) {
  if (nv == 0) return 1;
  int cnt = 0;
  for (int mask = 1; mask < (1 << nv); ++mask) {
    bool clique = true;
    for (int i = 0; i < nv && clique; ++i)
      if (mask >> i & 1)
        for (int j = i + 1; j < nv; ++j)
          if ((mask >> j & 1) && !(adj[i] >> j & 1)) clique = false;
    if (!clique) continue;
    bool maximal = true;
    for (int v = 0; v < nv && maximal; ++v)
      if (!(mask >> v & 1)) {
        bool adj_all = true;
        for (int i = 0; i < nv; ++i)
          if ((mask >> i & 1) && !(adj[v] >> i & 1)) adj_all = false;
        if (adj_all) maximal = false;
      }
    if (maximal) ++cnt;
  }
  return cnt;
}

TEST(Clique, CountVsBrute) {
  auto run = [](int nv, vector<pair<int, int>> ed) {
    n = nv;
    memset(E, 0, sizeof E);
    for (auto [a, b] : ed) {
      E[a] |= bit(b);
      E[b] |= bit(a);
    }
    vector<ull> adj(nv);
    for (int i = 0; i < nv; ++i) adj[i] = E[i];
    int got = count_cliques();
    EXPECT_EQ(got, brute_cliques(nv, adj));
  };
  run(0, {});
  run(1, {});
  run(3, {{0, 1}, {1, 2}, {2, 0}});
  run(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}});
  run(3, {});
}
