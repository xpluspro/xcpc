#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

using LL = long long;
using std::function;
using std::fill;
using std::pair;
using std::reverse;
using std::vector;
const int p = 998244353, MAXN = 1 << 12, inv_2 = (p + 1) / 2;
const int MOD = p, LOG = 63;
int inv[MAXN];

int qpow(int a, long long b) {
	int ans = 1;
	for (; b; b >>= 1, a = (LL)a * a % p) if (b & 1) ans = (LL)ans * a % p;
	return ans;
}
void ntt(int* a, int n, int type) {
	for (int i = 1, j = 0; i < n; ++i) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		j ^= bit; if (i < j) std::swap(a[i], a[j]);
	}
	for (int len = 2; len <= n; len <<= 1) {
		int root = qpow(3, (p - 1) / len);
		if (type == -1) root = qpow(root, p - 2);
		for (int i = 0; i < n; i += len) {
			int w = 1;
			for (int j = 0; j < len / 2; ++j) {
				int u = a[i + j], v = (LL)a[i + j + len / 2] * w % p;
				a[i + j] = u + v < p ? u + v : u + v - p;
				a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + p;
				w = (LL)w * root % p;
			}
		}
	}
	if (type == -1) { int ni = qpow(n, p - 2); for (int i = 0; i < n; ++i) a[i] = (LL)a[i] * ni % p; }
}
void ntt_init(int) {}

#include "../src/sections/Polynomial/assets/Math/多项式运算.cpp"
#include "../src/sections/Polynomial/assets/Math/多项式取模.cpp"
#include "../src/sections/Polynomial/assets/Math/快速线性递推-多项式取模.cpp"
#include "../src/sections/Polynomial/assets/Math/多点求值.cpp"
#include "../src/sections/Polynomial/assets/Math/快速线性递推-bostan-mori.cpp"
#include "../src/sections/Polynomial/assets/Math/线性递推.cpp"

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
	auto inverse = poly_inv(poly{1,1,1,1,1});
	auto identity = poly_mul(poly{1,1,1,1,1}, inverse);
	assert(identity[0] == 1); for (int i=1;i<5;++i) assert(identity[i] == 0);
	auto exponential = poly_exp(poly{0,1,0,0,0});
	assert((exponential == poly{1,1,(p+1)/2,166374059,291154603}));
	poly square_input{1,2,1,0,0};
	auto square_root = poly_sqrt(square_input);
	auto square = poly_mul(square_root, square_root);
	assert((vector<int>(square.begin(), square.begin()+5) == square_input));

	linear_recurrence at0(poly{0,1,1},0), at1(poly{0,1,1},1);
	assert(at0(vector<int>{0,1}) == 0);
	assert(at1(vector<int>{0,1}) == 1);

	poly_eval evaluation(poly{1,2,3,4}, vector<int>{1,2});
	assert((evaluation() == vector<int>{10,49}));
	assert((evaluation() == vector<int>{10,49}));

	const long long index = 1000000000000LL;
	assert(linear_recurrance(index, poly{0,1,1}, poly{0,1}) == fib(index));
	static_assert(std::is_same_v<decltype(&LinearRec::calc), int (LinearRec::*)(long long)>);
	poly fib_first{1,1}, fib_transition{1,1};
	LinearRec fib_recurrence(fib_first, fib_transition);
	assert(fib_recurrence.calc(index) == fib(index));

	using namespace kth_fwt;
	n=3; omega[0]=1; omega[1]=power(13,(kth_fwt::MOD-1)/3); omega[2]=1LL*omega[1]*omega[1]%kth_fwt::MOD;
	int values[3]={kth_fwt::MOD-1,kth_fwt::MOD-2,kth_fwt::MOD-3}, expected[3];
	for(int t=0;t<3;++t){ long long s=0; for(int y=0;y<3;++y) s=(s+1LL*values[y]*omega[t*y%3])%kth_fwt::MOD; expected[t]=(int)s; }
	fwt(values,3,1); for(int i=0;i<3;++i) assert(values[i]==expected[i]);
}
