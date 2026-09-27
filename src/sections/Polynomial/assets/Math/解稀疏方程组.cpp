// A 为模素数 p 下可逆的 n*n 矩阵；下标从 0 开始，数值在 [0,p)。
// 每次尝试 O(n*nnz(A)+n^2) 时间、O(n) 额外空间；验算失败重试。
vector<int> solve_sparse_equations(
	const vector<tuple<int, int, int> > &A, const vector<int> &b,
	unsigned seed = random_device{}()) { // 可指定 seed 复现
	int n = (int)b.size();
	if (all_of(b.begin(), b.end(), [](int x) { return x == 0; })) return b;
	auto mul = [&](const vector<int> &u, vector<int> &v) {
		fill(v.begin(), v.end(), 0);
		for (auto [x, y, z] : A) // [row, column, value]
			v[x] = (v[x] + (LL)u[y] * z) % p; };
	mt19937 gen(seed); uniform_int_distribution<int> dis(0, p - 1);
	while (true) {
		vector<int> w(n), a(2 * n), u = b, v(n), ans(n);
		for (auto &x : w) x = dis(gen);
		for (int i = 0; i < 2 * n; i++) {
			for (int j = 0; j < n; j++)
				a[i] = (a[i] + (LL)u[j] * w[j]) % p;
			if (i + 1 < 2 * n) { mul(u, v); u.swap(v); } }
		auto c = berlekamp_massey(a); int L = (int)c.size() - 1;
		if (c[L] == 0) continue;
		u = b; // 再次生成 A^i*b，累加解的系数
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < n; j++)
				ans[j] = (ans[j] + (LL)c[L - 1 - i] * u[j]) % p;
			if (i + 1 < L) { mul(u, v); u.swap(v); } }
		int scale = qpow(p - c[L], p - 2);
		for (auto &x : ans) x = (LL)x * scale % p;
		mul(ans, v);
		if (v == b) return ans; // 投影丢失信息时，换 w 重试
	} }
