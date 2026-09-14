#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <complex>
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

namespace primitive_root {
#include "../src/sections/NumberTheory/assets/Math/原根.cpp"
}

namespace pell {
#include "../src/sections/NumberTheory/assets/Math/Pell方程.cpp"
}

namespace cubic {
#include "../src/sections/NumberTheory/assets/Math/解一元三次方程.cpp"
}

#include "../src/sections/NumberTheory/assets/Math/min_25筛.cpp"

LL choose_mod(int n, int k, int mod) {
	if (k < 0 || k > n) return 0;
	std::vector<LL> row(k + 1); row[0] = 1;
	for (int i = 1; i <= n; ++i)
		for (int j = std::min(i, k); j; --j) row[j] = (row[j] + row[j - 1]) % mod;
	return row[k];
}

std::pair<ULL, LL> naive_factorial_without_p(int n, int p, int q) {
	ULL pk = 1;
	for (int i = 0; i < q; ++i) pk *= (ULL)p;
	ULL a = 1; LL b = 0;
	for (int i = 1; i <= n; ++i) {
		int x = i;
		while (x % p == 0) x /= p, ++b;
		a = (ULL)((u128)a * (ULL)x % pk);
	}
	return {a, b};
}

LL naive_min25_value(int n) {
	LL result = 1;
	for (int p = 2; 1LL * p * p <= n; ++p) {
		if (n % p != 0) continue;
		LL power = 1;
		while (n % p == 0) n /= p, power *= p;
		result *= power * (power - 1);
	}
	if (n > 1) result *= (LL)n * (n - 1);
	return result;
}

int main() {
	auto check_cubic = [](long double a, long double b, long double c, long double d) {
		auto roots = cubic::solveCubic(a, b, c, d);
		for (auto x : roots)
			assert(abs(((a * x + b) * x + c) * x + d) < 1e-12L);
		assert(abs(roots[0] + roots[1] + roots[2] + b / a) < 1e-12L);
		assert(abs(roots[0] * roots[1] + roots[1] * roots[2] +
			roots[2] * roots[0] - c / a) < 1e-12L);
		assert(abs(roots[0] * roots[1] * roots[2] + d / a) < 1e-12L);
	};
	check_cubic(1, -6, 11, -6); // 1, 2, 3
	check_cubic(1, 0, -3, 2);   // -2, 1, 1
	check_cubic(1, -3, 3, -1);  // 1, 1, 1
	check_cubic(1, 0, 0, -1);   // 一个实根，两个复根
	check_cubic(1, 0, -1, 1);

	assert(pell::peLL(2) == make_pair(3LL, 2LL));
	assert(pell::peLL(13) == make_pair(649LL, 180LL));
	assert(pell::peLL(61) == make_pair(1766319049LL, 226153980LL));
	assert(pell::peLL(109) == make_pair(158070671986249LL, 15140424455100LL));
	assert(pell::peLL(100000019999999LL) == make_pair(100000020000000LL, 10000001LL));
	assert(pell::peLL(4) == make_pair(-1LL, -1LL));
	assert(pell::peLL(661) == make_pair(-1LL, -1LL));

	LL expected_min25_sum = 0;
	for (int upper = 1; upper <= 300; ++upper) {
		expected_min25_sum += naive_min25_value(upper);
		min25::n = upper;
		min25::mod = 1000000007;
		min25::sq = 0;
		while (1LL * (min25::sq + 1) * (min25::sq + 1) <= upper)
			++min25::sq;
		min25::p.clear();
		for (int candidate = 2; candidate <= min25::sq; ++candidate) {
			bool is_prime_value = true;
			for (int divisor = 2; 1LL * divisor * divisor <= candidate; ++divisor)
				if (candidate % divisor == 0) is_prime_value = false;
			if (is_prime_value) min25::p.push_back(candidate);
		}
		min25::w.assign(1, 0);
		for (int x = 1; x <= min25::sq; ++x) min25::w.push_back(x);
		for (int divisor = min25::sq; divisor; --divisor)
			if (upper / divisor > min25::sq)
				min25::w.push_back(upper / divisor);
		min25::g1.assign(min25::w.size(), 0);
		min25::g2.assign(min25::w.size(), 0);
		for (int j = 1; j < (int)min25::w.size(); ++j)
			for (LL x = 2; x <= min25::w[j]; ++x) {
				min25::g1[j] = min25::md(min25::g1[j] + x);
				min25::g2[j] = min25::md(min25::g2[j] + x * x);
			}
		assert(min25::solve() == expected_min25_sum % min25::mod);
	}

	for (int p = 2; p <= 5000; ++p) {
		bool is_prime_value = true;
		for (int divisor = 2; 1LL * divisor * divisor <= p; ++divisor)
			if (p % divisor == 0) is_prime_value = false;
		if (!is_prime_value) continue;
		int root = primitive_root::primitive_root_prime(p);
		std::vector<bool> seen(p);
		long long value = 1;
		for (int exponent = 0; exponent < p - 1; ++exponent) {
			assert(!seen[value]);
			seen[value] = true;
			value = value * root % p;
		}
		assert(value == 1);
		for (int residue = 1; residue < p; ++residue) assert(seen[residue]);
	}
	assert(primitive_root::primitive_root_prime(998244353) == 3);
	assert(primitive_root::primitive_root_prime(1000000007) == 5);

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
	std::vector<LL> factors;
	primality::factor(LLONG_MAX, factors);
	u128 product = 1;
	for (LL factor : factors) {
		assert(primality::is_prime.solve(factor));
		product *= (ULL)factor;
	}
	assert(product == (ULL)LLONG_MAX);

	for (int p : {2, 3, 5, 7, 11})
		for (int q = 1; q <= 6; ++q)
			for (int n = 0; n <= 250; ++n)
				assert(factorial_mod::factorial_mod_fact(n, p, q) ==
					naive_factorial_without_p(n, p, q));
	assert(factorial_mod::factorial_mod_fact(131, 3, 1).first == 1);
	assert(factorial_mod::factorial_mod_fact(250, 3, 40) ==
		naive_factorial_without_p(250, 3, 40));
	assert(factorial_mod::factorial_mod_fact(250, 2, 63) ==
		naive_factorial_without_p(250, 2, 63));
	auto edge = factorial_mod::factorial_mod_fact(LLONG_MAX, 2, 3);
	assert(edge.first < 8 && edge.first % 2 == 1);
	assert(edge.second == LLONG_MAX - 63);
}
