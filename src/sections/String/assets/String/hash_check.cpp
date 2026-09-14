using u128 = __uint128_t;
using i128 = __int128_t;

// 可换成上方表格中的任意奇质数；偶数在模 2^128 下没有逆元。
static constexpr u128 CHECK_MOD = 1000000000000000003ULL;
static_assert(CHECK_MOD & 1);
static constexpr u128 inv = []() {
	u128 ret = CHECK_MOD;
	for (int i = 0; i < 6; i++) ret *= 2 - ret * CHECK_MOD;
	return ret; }();
static constexpr u128 chk = u128(-1) / CHECK_MOD;

// 等价于 |a-b| % CHECK_MOD == 0，且不会在有符号减法中溢出。
bool check(i128 a, i128 b) {
	u128 d = a >= b ? u128(a) - u128(b) : u128(b) - u128(a);
	return d * inv <= chk; }
