// 快速 Walsh-Hadamard 变换（FWHT），用于 XOR 卷积。
// 需要预先定义奇素数模数 MOD；数组长度 n 必须是 2 的幂，且系数在 [0, MOD) 内。
// 正变换和逆变换都使用同一个蝶形，逆变换额外乘以 n^{-1}。
void FWHT(int *a, int n, bool inverse = false) {
	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				int x = a[i + j], y = a[i + j + len];
				int sum = x + y;
				if (sum >= MOD) sum -= MOD;
				int difference = x - y;
				if (difference < 0) difference += MOD;
				a[i + j] = sum;
				a[i + j + len] = difference;
			}
		}
	}
	if (inverse) {
		int inv_n = qpow(n, MOD - 2);
		for (int i = 0; i < n; ++i)
			a[i] = (long long)a[i] * inv_n % MOD;
	}
}

void fwht(int *a, int n, bool inverse = false) { FWHT(a, n, inverse); }

// XOR 卷积：c[k] = sum_{i ^ j = k} a[i] * b[j]。
// 调用前将 a、b 补零到同一个不小于两者长度的 2 的幂；结果写回 a。
void xor_convolution(int *a, int *b, int n) {
	FWHT(a, n);
	FWHT(b, n);
	for (int i = 0; i < n; ++i)
		a[i] = (long long)a[i] * b[i] % MOD;
	FWHT(a, n, true);
}
