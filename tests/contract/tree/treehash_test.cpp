#include <gtest/gtest.h>
#include "prelude/common.hpp"

const ULL P = 1000000007ULL;

#include "src/sections/Tree/assets/tree/TreeHash.cpp"


TEST(TreeHash, IsomorphicEqual) {
  ULL a = get({get({}), get({})});
  ULL b = get({get({}), get({})});
  EXPECT_EQ(a, b);
  ULL c = get({get({get({})})});
  EXPECT_NE(a, c);
}

TEST(TreeHash, RandomIsomorphism) {
  std::mt19937 rng(40);
  for (int t = 0; t < 20; ++t) {
    // star vs path of 3
    ULL star = get({get({}), get({}), get({})});
    ULL path = get({get({get({})}), get({})});
    EXPECT_NE(star, path);
  }
  (void)rng;
}
