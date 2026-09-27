// 按 MTT 头部说明复制并适配的 FFT，仅供测试。
// 手册 5.1 节的 FFT.cpp 保持 double。
using cp = complex<long double>;
const long double PI = acosl(-1.0L);
static_assert(numeric_limits<long double>::radix == 2 &&
	numeric_limits<long double>::digits >= 64);
vector<cp> omega[25];
void fft_init(int n) {
	for (int k = 2, d = 0; k <= n; k *= 2, d++) {
		omega[d].resize(k + 1);
		for (int i = 0; i <= k; i++)
			omega[d][i] = polar(1.0L, 2 * PI * i / k); } }
void fft(cp* a, int n, int t) {
	for (int i = 1, j = 0; i < n - 1; i++) {
		int k = n; do j ^= (k >>= 1); while (j < k);
		if (i < j) swap(a[i], a[j]); }
	for (int k = 1, d = 0; k < n; k *= 2, d++)
		for (int i = 0; i < n; i += k * 2)
			for (int j = 0; j < k; j++) {
				cp w = omega[d][t > 0 ? j : k * 2 - j];
				cp u = a[i + j], v = w * a[i + j + k];
				a[i + j] = u + v; a[i + j + k] = u - v; }
	if (t < 0) for (int i = 0; i < n; i++) a[i] /= n; }
