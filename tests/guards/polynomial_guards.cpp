#include "prelude/death.hpp"
#define N 256
#define MAXN 256
#include "prelude/common.hpp"

const int p = 998244353;
int inv_2;
int inv[MAXN];
inline int qpow(int a, int k) { return (int)qpow((LL)a, (LL)k, (LL)p); }

#include "src/sections/Polynomial/assets/Math/NTT.cpp"
#include "src/sections/Polynomial/assets/Math/多项式运算.cpp"
#include "src/sections/Polynomial/assets/Math/多项式取模.cpp"
#include "src/sections/Polynomial/assets/Math/快速线性递推-多项式取模.cpp"

TEST(Guards, NttInitNotPowerOfTwo) { XCPC_EXPECT_ASSERT(ntt_init(6)); }

TEST(Guards, NttNegativeResidue) {
  ntt_init(4);
  int c[4] = {0, 1, -1, 0};
  XCPC_EXPECT_ASSERT(ntt(c, 4, 1));
}

TEST(Guards, PolyInvZeroConst) {
  ntt_init(8);
  poly a = {0, 1, 0, 0};
  XCPC_EXPECT_ASSERT((void)poly_inv(a));
}

TEST(Guards, PolyExpNonzeroConst) {
  ntt_init(8);
  poly a = {1, 1, 0, 0};
  XCPC_EXPECT_ASSERT((void)poly_exp(a));
}

TEST(Guards, PolyCalcUnequal) {
  ntt_init(8);
  poly a = {1, 0}, b = {1, 0, 0, 0};
  XCPC_EXPECT_ASSERT((void)poly_mul(a, b));
}

TEST(Guards, LinearRecC0) {
  ntt_init(16);
  poly c = {1, 1, 1};
  XCPC_EXPECT_ASSERT((void)linear_recurrence(c, 3));
}

TEST(Guards, LinearRecSize) {
  ntt_init(16);
  poly c = {0, 1, 1};
  linear_recurrence lr(c, 3);
  vector<int> a = {1};
  XCPC_EXPECT_ASSERT((void)lr(a));
}
