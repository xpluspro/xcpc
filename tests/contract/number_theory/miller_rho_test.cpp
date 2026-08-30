#include <gtest/gtest.h>
#include "prelude/common.hpp"

static LL gcd_ll(LL a, LL b) { return b ? gcd_ll(b, a % b) : abs(a); }
#define gcd gcd_ll

#include "src/sections/NumberTheory/assets/Math/Miller Rabin And Pollard Rho.cpp"


static bool naive_prime(LL n) {
  if (n < 2) return false;
  for (LL i = 2; i * i <= n; ++i)
    if (n % i == 0) return false;
  return true;
}

TEST(Miller, TinyAndTables) {
  for (int i = 0; i <= 200; ++i) EXPECT_EQ(is_prime.solve(i), naive_prime(i));
  EXPECT_FALSE(is_prime.solve(1));
  EXPECT_FALSE(is_prime.solve(0));
  EXPECT_TRUE(is_prime.solve(2));
  EXPECT_TRUE(is_prime.solve(3));
  EXPECT_FALSE(is_prime.solve(4));
  EXPECT_FALSE(is_prime.solve(561)); // Carmichael
  EXPECT_FALSE(is_prime.solve(1105));
  // {2,7,61} is deterministic below 4,759,123,141
  vector<LL> primes = {97, 997, 10007, 100003, 1000003, 2147483647};
  for (LL p : primes) EXPECT_TRUE(is_prime.solve(p));
  EXPECT_FALSE(is_prime.solve(1000000007LL * 1000000009LL > 0
                                  ? 1000000007LL * 3
                                  : 0));
  EXPECT_FALSE(is_prime.solve(1000000007LL * 3));
}

TEST(Rho, FactorSmall) {
  vector<LL> empty;
  factor(0, empty);
  factor(1, empty);
  EXPECT_TRUE(empty.empty());

  for (LL n = 2; n <= 2000; ++n) {
    vector<LL> d;
    factor(n, d);
    LL prod = 1;
    for (LL x : d) {
      EXPECT_TRUE(is_prime.solve(x));
      prod *= x;
    }
    EXPECT_EQ(prod, n);
  }
  vector<LL> d;
  factor(10007LL * 10009LL, d);
  EXPECT_EQ(d.size(), 2u);
  sort(d.begin(), d.end());
  EXPECT_EQ(d[0], 10007);
  EXPECT_EQ(d[1], 10009);
}
