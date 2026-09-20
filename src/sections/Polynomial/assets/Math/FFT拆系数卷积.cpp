// 需要先抄 FFT；有符号 32 位整数卷积，答案需在 long long 范围内。
vector<long long> convolution_ll(const vector<int>& a, const vector<int>& b) {
	if (a.empty() || b.empty()) return {};
	const long long B = 1 << 15;
	int need = (int)a.size() + (int)b.size() - 1, n = 1;
	while (n < need) n <<= 1;
	vector<cp> a0(n), a1(n), b0(n), b1(n), c0(n), c1(n), c2(n);
	for (int i = 0; i < (int)a.size(); i++)
		a0[i] = a[i] % B, a1[i] = a[i] / B;
	for (int i = 0; i < (int)b.size(); i++)
		b0[i] = b[i] % B, b1[i] = b[i] / B;
	fft_init(n);
	fft(a0.data(), n, 1); fft(a1.data(), n, 1);
	fft(b0.data(), n, 1); fft(b1.data(), n, 1);
	for (int i = 0; i < n; i++) {
		c0[i] = a0[i] * b0[i];
		c1[i] = a0[i] * b1[i] + a1[i] * b0[i];
		c2[i] = a1[i] * b1[i]; }
	fft(c0.data(), n, -1); fft(c1.data(), n, -1);
	fft(c2.data(), n, -1);
	vector<long long> c(need);
	for (int i = 0; i < need; i++) {
		__int128 x = llround(c0[i].real());
		x += (__int128)llround(c1[i].real()) * B;
		x += (__int128)llround(c2[i].real()) * B * B;
		c[i] = (long long)x; }
	return c;
}
