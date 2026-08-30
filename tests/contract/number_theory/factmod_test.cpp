#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/Factorial Mod.cpp"


TEST(FactMod, SmallExact) {
  // Val is documented to do +/* modulo p^q, so compare the p-free part mod p^q,
  // not the unreduced integer.
  prepare(8);
  for (LL p : {2, 3, 5}) {
    for (LL q = 1; q <= 3; ++q) {
      LL pq = 1;
      for (LL i = 0; i < q; ++i) pq *= p;
      for (LL n = 0; n <= 12; ++n) {
        auto [A, B] = fact(n, p, q);
        __int128 fac = 1;
        LL exp = 0;
        for (int i = 1; i <= n; ++i) {
          int x = i;
          while (x % p == 0) x /= (int)p, ++exp;
          fac *= x;
        }
        EXPECT_EQ(B, exp) << n << " " << p << " " << q;
        EXPECT_EQ((unsigned long long)(A % pq), (unsigned long long)(fac % pq))
            << n << " " << p << " " << q;
      }
    }
  }
}
