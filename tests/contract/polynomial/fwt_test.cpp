#include <gtest/gtest.h>
#define XCPC_M_IS_MOD
#define M 998244353
#define N 1024
#include "prelude/common.hpp"
#include "src/sections/Polynomial/assets/Math/FWT.cpp"


static const LL ANDC[2][2] = {{1, 1}, {0, 1}};
static const LL iAND[2][2] = {{1, M - 1}, {0, 1}};
static const LL ORC[2][2] = {{1, 0}, {1, 1}};
static const LL iOR[2][2] = {{1, 0}, {M - 1, 1}};

static vector<LL> conv_or(const vector<LL> &a, const vector<LL> &b) {
  int n = (int)a.size();
  vector<LL> c(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) c[i | j] = (c[i | j] + a[i] * b[j]) % M;
  return c;
}
static vector<LL> conv_and(const vector<LL> &a, const vector<LL> &b) {
  int n = (int)a.size();
  vector<LL> c(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) c[i & j] = (c[i & j] + a[i] * b[j]) % M;
  return c;
}
static vector<LL> conv_xor(const vector<LL> &a, const vector<LL> &b) {
  int n = (int)a.size();
  vector<LL> c(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) c[i ^ j] = (c[i ^ j] + a[i] * b[j]) % M;
  return c;
}

TEST(Fwt, OrAndXor) {
  for (int n : {2, 4, 8, 16}) {
    vector<LL> a(n), b(n);
    for (int i = 0; i < n; ++i) a[i] = i + 1, b[i] = n - i;
    auto run = [&](const LL C[2][2], const LL iC[2][2], auto conv) {
      vector<LL> fa = a, fb = b;
      FWT(fa.data(), C, n);
      FWT(fb.data(), C, n);
      vector<LL> fc(n);
      for (int i = 0; i < n; ++i) fc[i] = fa[i] * fb[i] % M;
      FWT(fc.data(), iC, n);
      auto want = conv(a, b);
      for (int i = 0; i < n; ++i) EXPECT_EQ((fc[i] % M + M) % M, want[i]);
    };
    run(ORC, iOR, conv_or);
    run(ANDC, iAND, conv_and);
    run(XOR, iXOR, conv_xor);
  }
}
