using u128 = __uint128_t;
using i128 = __int128_t;
const int P = 1e9 + 7; // any prime number
static constexpr u128 inv = []() {
	u128 ret = P;
	for (int i = 0; i < 6; i++) ret *= 2 - ret * P;
	return ret; }();
constexpr u128 chk = u128(-1) / P;
bool check(i128 a, i128 b) { // 注意：i128
	if (a < b) swap(a, b);
	return (a - b) * inv <= chk; }