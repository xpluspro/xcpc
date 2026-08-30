#include "prelude/common.hpp"
#include "prelude/death.hpp"

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

TEST(Guards, CliqueSelfLoopAborts) {
  n = 2;
  memset(E, 0, sizeof E);
  E[0] |= bit(0);
  XCPC_EXPECT_ASSERT((void)count_cliques());
}
