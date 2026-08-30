#include "prelude/death.hpp"
#define N 64
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Math/exgcd.cpp"
#include "src/sections/NumberTheory/assets/Math/Pell方程.cpp"
#include "src/sections/NumberTheory/assets/Math/直线下格点统计.cpp"

int MOD = 13;
#define power qpow
#include "src/sections/NumberTheory/assets/Math/平方剩余.cpp"

TEST(Guards, InvNotCoprimeAborts) { XCPC_EXPECT_ASSERT(inv(2, 4)); }

TEST(Guards, PellNonPositiveAborts) { XCPC_EXPECT_ASSERT((void)peLL(0)); }

TEST(Guards, FloorSumNonPositiveAborts) {
  XCPC_EXPECT_ASSERT((void)solve(0, 1, 1, 1));
  XCPC_EXPECT_ASSERT((void)solve(1, 1, 1, 0));
}

TEST(Guards, TonelliOutOfRangeAborts) {
  MOD = 13;
  int x;
  XCPC_EXPECT_ASSERT((void)solve(-1, x));
  XCPC_EXPECT_ASSERT((void)solve(13, x));
}
