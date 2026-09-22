#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstring>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

using LL = long long;
using ULL = unsigned long long;
using std::function;
using std::fill;
using std::pair;
using std::reverse;
using std::swap;
using std::vector;
const int p = 998244353, MAXN = 1 << 12, inv_2 = (p + 1) / 2;
const int N = MAXN;
const int MOD = p, LOG = 63;

int qpow(int a, long long b) {
	int ans = 1;
	for (; b; b >>= 1, a = (LL)a * a % p) if (b & 1) ans = (LL)ans * a % p;
	return ans;
}
#include "../src/sections/Polynomial/assets/Math/NTT.cpp"
int inv[MAXN];

#include "../src/sections/Polynomial/assets/Math/多项式运算.cpp"
#include "../src/sections/Polynomial/assets/Math/多项式取模.cpp"
#include "../src/sections/Polynomial/assets/Math/快速线性递推-多项式取模.cpp"
#include "../src/sections/Polynomial/assets/Math/多点求值.cpp"
#include "../src/sections/Polynomial/assets/Math/快速线性递推-bostan-mori.cpp"
#include "../src/sections/Polynomial/assets/Math/线性递推.cpp"

namespace arbitrary_mod_convolution {
using namespace std;
using namespace std::complex_literals;
#include "mtt_fft_long_double.hpp"
#include "../src/sections/Polynomial/assets/Math/MTT.cpp"
}

namespace kth_fwt {
const int MOD = 1000000009, K = 8;
int n, omega[K];
int power(int a, long long b) { int r=1; for(;b;b>>=1,a=1LL*a*a%MOD)if(b&1)r=1LL*r*a%MOD; return r; }
int inv(int x) { return power(x, MOD - 2); }
#include "../src/sections/Polynomial/assets/tbr/fwt.cpp"
}

int fib(long long n) {
	if (!n) return 0;
	auto rec = [](auto&& self, long long k) -> pair<int,int> {
		if (!k) return {0,1};
		auto [a,b] = self(self,k/2);
		int c = (LL)a * ((2LL*b-a+p)%p) % p;
		int d = ((LL)a*a+(LL)b*b)%p;
		return k&1 ? pair<int,int>{d,(c+d)%p} : pair<int,int>{c,d};
	};
	return rec(rec,n).first;
}

int main() {
	inv[1]=1; for(int i=2;i<MAXN;++i) inv[i]=(LL)(p-p/i)*inv[p%i]%p;
	auto product = poly_mul(poly{1,2,3}, poly{4,5});
	assert((vector<int>(product.begin(), product.begin()+4) == vector<int>{4,13,22,15}));
	auto arbitrary_product = arbitrary_mod_convolution::multiply(
		vector<int>{1,2,3}, vector<int>{4,5}, 1000000007);
	assert((arbitrary_product == vector<int>{4,13,22,15}));
	auto inverse = poly_inv(poly{1,1,1,1,1});
	auto identity = poly_mul(poly{1,1,1,1,1}, inverse);
	assert(identity[0] == 1); for (int i=1;i<5;++i) assert(identity[i] == 0);
	auto exponential = poly_exp(poly{0,1,0,0,0});
	assert((exponential == poly{1,1,(p+1)/2,166374059,291154603}));
	poly square_input{1,2,1,0,0};
	auto square_root = poly_sqrt(square_input);
	auto square = poly_mul(square_root, square_root);
	assert((vector<int>(square.begin(), square.begin()+5) == square_input));

	auto check_division = [](const poly& dividend, const poly& divisor,
		const poly& remainder, const poly& quotient) {
		assert(poly_div(dividend, divisor) == quotient);
		auto result = poly_mod(dividend, divisor);
		assert(result.first == remainder && result.second == quotient);
	};
	// poly_mul 的高次补零不能影响除法的次数判断或反转求逆。
	poly padded_divisor = poly_mul(poly{1,1}, poly{1,1});
	assert((padded_divisor == poly{1,2,1,0}));
	check_division(poly{1,2,1}, padded_divisor, poly{0,0}, poly{1});
	check_division(poly{1,0,0,1}, padded_divisor, poly{3,3}, poly{p-2,1});
	check_division(poly{2}, padded_divisor, poly{2}, poly{});
	check_division(poly{}, padded_divisor, poly{}, poly{});
	check_division(poly{0,0,0}, padded_divisor, poly{0,0}, poly{0});
	// 常数除数、低次端为零的除数，以及被除式自身的高次补零。
	check_division(poly{2,4,6}, poly{2,0,0}, poly{}, poly{1,2,3});
	check_division(poly{3,4,5}, poly{0,1,0}, poly{3}, poly{4,5});
	check_division(poly{1,2,1,0,0}, padded_divisor, poly{0,0}, poly{1,0,0});
	// 大量无效补零也不应扩大取模内部的 NTT 长度。
	padded_divisor.resize(N + 1);
	check_division(poly{1,2,1}, padded_divisor, poly{0,0}, poly{1});
	poly zero_remainder = poly_mod(poly{7}, poly{1,0}).first;
	assert(poly_auto_mul(zero_remainder, zero_remainder).empty());
	assert(poly_auto_mul(poly{}, poly{1,2}).empty());
	assert(poly_auto_mul(poly{1,2}, poly{}).empty());

	linear_recurrence at0(poly{0,1,1},0), at1(poly{0,1,1},1);
	assert(at0(vector<int>{0,1}) == 0);
	assert(at1(vector<int>{0,1}) == 1);

	auto check_evaluation = [](const poly& f, const vector<int>& xs,
		const vector<int>& expected) {
		poly_eval evaluation(f, xs);
		assert(evaluation() == expected);
		assert(evaluation() == expected);
	};
	check_evaluation(poly{1,2,3,4}, vector<int>{1,2}, vector<int>{10,49});
	// 空向量表示零多项式；空询问、单点询问也必须安全。
	check_evaluation(poly{}, vector<int>{}, vector<int>{});
	check_evaluation(poly{}, vector<int>{7}, vector<int>{0});
	check_evaluation(poly{}, vector<int>{0,0,7}, vector<int>{0,0,0});
	check_evaluation(poly{0}, vector<int>{}, vector<int>{});
	check_evaluation(poly{0}, vector<int>{0,7}, vector<int>{0,0});
	check_evaluation(poly{5}, vector<int>{0,0,7}, vector<int>{5,5,5});
	// 零点、重复点及 -1 的模 p 表示；也覆盖内部补询问点的情况。
	check_evaluation(poly{1,2,3}, vector<int>{0}, vector<int>{1});
	check_evaluation(poly{1,2,3}, vector<int>{0,1,0,p-1}, vector<int>{1,6,1,2});

	const long long index = 1000000000000LL;
	assert(linear_recurrance(index, poly{0,1,1}, poly{0,1}) == fib(index));
	static_assert(std::is_same_v<decltype(&LinearRec::calc), int (LinearRec::*)(long long)>);
	poly fib_first{0,1}, fib_transition{1,1};
	LinearRec fib_recurrence(fib_first, fib_transition);
	assert(fib_recurrence.calc(index) == fib(index));
	// 三种接口统一查询从 0 开始的第 n 项，与逐项递推核对。
	auto check_recurrence_indices = [](poly first, poly trans) {
		poly c{0}; c.insert(c.end(), trans.begin(), trans.end());
		LinearRec rec(first, trans);
		poly expected = first;
		for (int n = (int)first.size(); n <= 32; ++n) {
			int value = 0;
			for (int j = 0; j < (int)trans.size(); ++j)
				value = (value + (LL)trans[j] * expected[n - 1 - j]) % p;
			expected.push_back(value);
		}
		for (int n = 0; n <= 32; ++n) {
			assert(rec.calc(n) == expected[n]);
			assert(linear_recurrance(n, c, first) == expected[n]);
			linear_recurrence at_n(c, n);
			assert(at_n(first) == expected[n]);
		}
	};
	check_recurrence_indices(poly{1,3}, poly{2,1});
	check_recurrence_indices(poly{5}, poly{3});
	check_recurrence_indices(poly{7}, poly{0});
	check_recurrence_indices(poly{2,4,8}, poly{1,0,1});
	// 下标平移不能通过 k+1 引入 long long 溢出。
	poly geometric_first{5}, geometric_transition{3};
	LinearRec geometric(geometric_first, geometric_transition);
	const long long max_index = std::numeric_limits<long long>::max();
	assert(geometric.calc(max_index) == (LL)5 * qpow(3, max_index) % p);

	kth_fwt::n=3; kth_fwt::omega[0]=1;
	kth_fwt::omega[1]=kth_fwt::power(13,(kth_fwt::MOD-1)/3);
	kth_fwt::omega[2]=1LL*kth_fwt::omega[1]*kth_fwt::omega[1]%kth_fwt::MOD;
	int values[3]={kth_fwt::MOD-1,kth_fwt::MOD-2,kth_fwt::MOD-3}, expected[3];
	for(int t=0;t<3;++t){ long long s=0; for(int y=0;y<3;++y) s=(s+1LL*values[y]*kth_fwt::omega[t*y%3])%kth_fwt::MOD; expected[t]=(int)s; }
	kth_fwt::fwt(values,3,1); for(int i=0;i<3;++i) assert(values[i]==expected[i]);
}
