// 需要先抄 FFT；每一路卷积需能可靠舍入，答案需在 long long 范围内。
vector<long long> convolution(const vector<int>& a, const vector<int>& b) {
	if (a.empty() || b.empty()) return {};
	int need = (int)a.size() + (int)b.size() - 1, n = 1;
	while (n < need) n <<= 1;
	vector<cp> x(n), y(n);
	for (int i = 0; i < (int)a.size(); i++) x[i] = a[i];
	for (int i = 0; i < (int)b.size(); i++) y[i] = b[i];
	fft_init(n);
	fft(x.data(), n, 1); fft(y.data(), n, 1);
	for (int i = 0; i < n; i++) x[i] *= y[i];
	fft(x.data(), n, -1);
	vector<long long> c(need);
	for (int i = 0; i < need; i++) c[i] = llround(x[i].real());
	return c;
}
