#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/exgcd.cpp"

struct U {
  LL x, z;
};

int M;

#include "src/sections/NumberTheory/assets/Math/扩展卢卡斯.cpp"


TEST(ExLucas, PrimeModulusMatches) {
  // fac() reduces with the global M, which must equal the modulus argument.
  // Oracle: exact C(n,k) % mod (Fermat-product oracles are wrong once n>=mod).
  auto C = [](int n, int m) -> LL {
    if (m < 0 || m > n) return 0;
    LL r = 1;
    for (int i = 1; i <= m; ++i) r = r * (n - m + i) / i;
    return r;
  };
  vector<int> mods = {13, 17, 19};
  for (int mod : mods) {
    for (int n = 0; n <= 12; ++n) {
      for (int m = 0; m <= n; ++m) {
        l = 0;
        M = 1; // must not be required; the argument is the modulus
        LL got = exLucas(n, m, mod);
        EXPECT_EQ((got % mod + mod) % mod, C(n, m) % mod)
            << n << " " << m << " " << mod;
      }
    }
  }
}

TEST(ExLucas, CompositeSmallExact) {
  // n! fits and we compare against exact C(n,m) % M.
  auto C = [](int n, int m) -> LL {
    if (m < 0 || m > n) return 0;
    LL r = 1;
    for (int i = 1; i <= m; ++i) r = r * (n - m + i) / i;
    return r;
  };
  for (int mod : {8, 9, 12, 15, 16, 18, 24}) {
    for (int n = 0; n <= 12; ++n)
      for (int k = 0; k <= n; ++k) {
        l = 0;
        M = 1;
        LL got = exLucas(n, k, mod);
        EXPECT_EQ((got % mod + mod) % mod, C(n, k) % mod)
            << n << " " << k << " " << mod;
      }
  }
}
