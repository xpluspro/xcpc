#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <limits>
#include <vector>

using namespace std;
using namespace std::complex_literals;
using LL = long long;
constexpr int MAXN = 1 << 19;

namespace ordinary_fft {
#include "../src/sections/Polynomial/assets/Math/FFT.cpp"
}

#include "mtt_fft_long_double.hpp"
#include "../src/sections/Polynomial/assets/Math/MTT.cpp"

int main() {
	ordinary_fft::cp a[4] = {1, 2, 3, 0}, b[4] = {4, 5, 0, 0};
	ordinary_fft::fft_init(4);
	ordinary_fft::fft(a, 4, 1); ordinary_fft::fft(b, 4, 1);
	for (int i = 0; i < 4; ++i) a[i] *= b[i];
	ordinary_fft::fft(a, 4, -1);
	const int expected[4] = {4, 13, 22, 15};
	for (int i = 0; i < 4; ++i) assert(llround(a[i].real()) == expected[i]);

	constexpr int mod = 1000000007, n = 1 << 18;
	vector<int> input(n, mod - 1);
	auto product = multiply(input, input, mod);
	assert(product.size() == 2 * n - 1);
	// 每一对系数的乘积模 mod 都为 1，答案等于参与求和的项数。
	// 原 double 版本在本机此例中有 8 个位置因精度出错。
	for (int i = 0; i < 2 * n - 1; ++i)
		assert(product[i] == min(i + 1, 2 * n - 1 - i));

	// 缩短长度后重用工作数组，覆盖非零的低位、高位和交叉项。
	auto small = multiply({mod - 1, mod - 2, 1}, {mod - 3, 4}, mod);
	assert((small == vector<int>{3, 2, mod - 11, 4}));
}
