using cp = complex<double>; const double PI = acos(-1.0);
vector<cp> omega[25]; // 单位根
// n 是 DFT 的最大长度，例如如果最多有两个长为 m 的多项式相乘，
// 或者求逆的长度为 m，那么 n 需要 >= 2m
void fft_init(int n) { // n = 2^k
	for (int k = 2, d = 0; k <= n; k *= 2, d++) {
		omega[d].resize(k + 1); 
		for (int i = 0; i <= k; i++) // polar 是用模和辐角求复数
			omega[d][i] = polar(1.0, 2 * PI * i / k); } }
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

// usage: 有符号 32 位整数卷积（答案需在 long long 范围内）
// const long long B = 1 << 15; // 按数据范围选，使每路卷积可可靠舍入
// int need = a.size() + b.size() - 1, n = 1;
// while (n < need) n <<= 1;
// vector<cp> a0(n), a1(n), b0(n), b1(n), c0(n), c1(n), c2(n);
// for (int i = 0; i < (int)a.size(); i++)
//     a0[i] = a[i] % B, a1[i] = a[i] / B;
// for (int i = 0; i < (int)b.size(); i++)
//     b0[i] = b[i] % B, b1[i] = b[i] / B;
// fft_init(n);
// fft(a0.data(), n, 1); fft(a1.data(), n, 1);
// fft(b0.data(), n, 1); fft(b1.data(), n, 1);
// for (int i = 0; i < n; i++) {
//     c0[i] = a0[i] * b0[i];
//     c1[i] = a0[i] * b1[i] + a1[i] * b0[i];
//     c2[i] = a1[i] * b1[i]; }
// fft(c0.data(), n, -1); fft(c1.data(), n, -1);
// fft(c2.data(), n, -1);
// vector<long long> c(need);
// for (int i = 0; i < need; i++) {
//     __int128 x = llround(c0[i].real());
//     x += (__int128)llround(c1[i].real()) * B;
//     x += (__int128)llround(c2[i].real()) * B * B;
//     c[i] = (long long)x; }
