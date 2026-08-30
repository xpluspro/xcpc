#include <gtest/gtest.h>
#define N 4096
#define MAXN 4096
#include "prelude/common.hpp"

const int p = 998244353;
int inv_2;
int inv[MAXN];
inline int qpow(int a, int k) { return (int)qpow((LL)a, (LL)k, (LL)p); }

#include "src/sections/Polynomial/assets/Math/NTT.cpp"
#include "src/sections/Polynomial/assets/Math/多项式运算.cpp"
#include "src/sections/Polynomial/assets/Math/多项式取模.cpp"
#include "src/sections/Polynomial/assets/Math/快速线性递推-bostan-mori.cpp"
#include "src/sections/Polynomial/assets/Math/快速线性递推-多项式取模.cpp"


static void ntt_prepare(int n) {
  ntt_init(n);
  inv_2 = qpow(2, p - 2, p);
  inv[1] = 1;
  for (int i = 2; i < MAXN; ++i) inv[i] = (LL)(p - p / i) * inv[p % i] % p;
}

static vector<int> naive_mul(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size() + b.size() - 1);
  for (size_t i = 0; i < a.size(); ++i)
    for (size_t j = 0; j < b.size(); ++j)
      c[i + j] = (c[i + j] + (LL)a[i] * b[j]) % p;
  return c;
}

TEST(Ntt, RoundTripAndMultiply) {
  ntt_prepare(1 << 12);
  for (int n : {2, 4, 8, 16, 32, 64}) {
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) a[i] = (i * 17 + 3) % p, b[i] = (i * 9 + 1) % p;
    vector<int> aa = a;
    ntt(aa.data(), n, 1);
    ntt(aa.data(), n, -1);
    for (int i = 0; i < n; ++i) EXPECT_EQ(aa[i], a[i]);
    auto got = poly_auto_mul(a, b);
    auto want = naive_mul(a, b);
    ASSERT_EQ(got.size(), want.size());
    for (size_t i = 0; i < want.size(); ++i) EXPECT_EQ(got[i], want[i]);
  }
}

TEST(Poly, InvMulIdentity) {
  ntt_prepare(1 << 12);
  for (int n : {1, 2, 4, 8, 16}) {
    poly a(n);
    a[0] = 3;
    for (int i = 1; i < n; ++i) a[i] = (i * 5 + 7) % p;
    poly b = poly_inv(a);
    poly c = poly_auto_mul(a, b);
    c.resize(n);
    EXPECT_EQ(c[0], 1);
    for (int i = 1; i < n; ++i) EXPECT_EQ(c[i], 0);
  }
}

TEST(Poly, LnExpInverse) {
  ntt_prepare(1 << 12);
  for (int n : {2, 4, 8, 16}) {
    poly f(n);
    f[0] = 0;
    for (int i = 1; i < n; ++i) f[i] = (i * 11 + 4) % p;
    poly g = poly_exp(f);
    poly h = poly_ln(g);
    h.resize(n);
    for (int i = 0; i < n; ++i) EXPECT_EQ(h[i], f[i]) << n << " " << i;
  }
}

TEST(Poly, DivMod) {
  ntt_prepare(1 << 12);
  poly a = {1, 2, 3, 4, 5, 6, 7, 8};
  poly b = {1, 1, 1};
  auto [r, d] = poly_mod(a, b);
  poly recon = poly_auto_mul(b, d);
  recon.resize(a.size());
  for (size_t i = 0; i < r.size(); ++i) recon[i] = (recon[i] + r[i]) % p;
  recon.resize(a.size());
  for (size_t i = 0; i < a.size(); ++i) EXPECT_EQ(recon[i] % p, a[i] % p);
}

TEST(LinRec, Fibonacci) {
  ntt_prepare(1 << 12);
  // F_n = F_{n-1} + F_{n-2}, F_0=0, F_1=1
  poly f = {0, 1, 1};
  poly init = {0, 1};
  vector<int> F(20);
  F[0] = 0;
  F[1] = 1;
  for (int i = 2; i < 20; ++i) F[i] = (F[i - 1] + F[i - 2]) % p;
  for (int n = 0; n < 20; ++n) {
    linear_recurrence lr(f, n);
    EXPECT_EQ(lr(init), F[n]) << n;
    EXPECT_EQ(linear_recurrance(n, f, init), F[n]) << n;
  }
}


