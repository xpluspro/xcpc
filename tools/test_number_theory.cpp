#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

using LL = long long;
using ULL = unsigned long long;
using u128 = __uint128_t;
using namespace std;
struct U { LL x, z; };

LL Pw(LL a, LL b, LL mod) {
	LL ans = 1;
	for (; b; b >>= 1, a = (LL)((__int128)a * a % mod))
		if (b & 1) ans = (LL)((__int128)ans * a % mod);
	return ans;
}
void exgcd(LL a, LL b, LL& x, LL& y) {
	if (!b) { x = 1; y = 0; return; }
	LL xx, yy; exgcd(b, a % b, xx, yy);
	x = yy; y = xx - a / b * yy;
}
LL inv(LL a, LL mod) { LL x, y; exgcd(a, mod, x, y); return (x % mod + mod) % mod; }
#define fr(i,l,r) for(int i=(l);i<=(r);++i)
namespace exlucas {
#include "../src/sections/NumberTheory/assets/Math/扩展卢卡斯.cpp"
}
#undef fr

namespace llfpm {
#include "../src/sections/NumberTheory/assets/Miscellany/LLFPM.cpp"
}

LL mul(LL a, LL b, LL mod) { return (LL)((__int128)a * b % mod); }
LL qpow(LL a, LL b, LL mod) {
	LL ans = 1;
	for (; b; b >>= 1, a = mul(a, a, mod)) if (b & 1) ans = mul(ans, a, mod);
	return ans;
}
namespace primality {
#include "../src/sections/NumberTheory/assets/Math/Miller Rabin And Pollard Rho.cpp"
}
#undef rand

namespace factorial_mod {
#define fact factorial_mod_fact
#include "../src/sections/NumberTheory/assets/Math/Factorial Mod.cpp"
#undef fact
}

LL choose_mod(int n, int k, int mod) {
	if (k < 0 || k > n) return 0;
	std::vector<LL> row(k + 1); row[0] = 1;
	for (int i = 1; i <= n; ++i)
		for (int j = std::min(i, k); j; --j) row[j] = (row[j] + row[j - 1]) % mod;
	return row[k];
}

int main() {
	for (int mod : {8, 9, 12, 25, 49, 72})
		for (int n = 0; n <= 30; ++n)
			for (int k = 0; k <= n; ++k) assert(exlucas::exLucas(n, k, mod) == choose_mod(n, k, mod));
	assert(exlucas::exLucas(20, 7, 72) == choose_mod(20, 7, 72));
	assert(exlucas::exLucas(10, 11, 12) == 0);

	std::mt19937_64 gen(20260831);
	const LL mod = 9223372036854775783LL;
	for (int i = 0; i < 10000; ++i) {
		LL a = (LL)(gen() % mod), b = (LL)(gen() % mod);
		assert(llfpm::modmul(a, b, mod) == (LL)((__int128)a * b % mod));
		assert(llfpm::modmul((ULL)a, (ULL)b, mod) == (ULL)((u128)(ULL)a * (ULL)b % (ULL)mod));
	}
	llfpm::DIV reducer; reducer.init(1000000000039ULL);
	assert(reducer.mod(2000000000077ULL) == 1000000000038ULL);

	assert(!primality::is_prime.solve(4759123141LL));
	assert(primality::is_prime.solve(2305843009213693951LL));
	assert(!primality::is_prime.solve(341550071728321LL));

	std::fill(&factorial_mod::choo[0][0], &factorial_mod::choo[0][0] + 70 * 70, 0);
	(void)factorial_mod::factorial_mod_fact(1, 2, 3);
	assert(factorial_mod::choo[2][1] == 2);
}
