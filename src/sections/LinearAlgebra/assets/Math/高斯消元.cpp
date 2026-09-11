// a 是 m * (n + 1) 的增广矩阵；返回 {秩, 一组解}
// 秩为 -1 表示无解，秩为 n 表示唯一解，否则有无穷多解
pair<int, vector<LD>> gauss(vector<vector<LD>> a, LD eps = 1e-12L) {
	if (a.empty()) return {0, {}};
	int m = (int)a.size(), n = (int)a[0].size() - 1, row = 0;
	vector<int> pivot(n, -1);
	for (int col = 0; col < n && row < m; col++) {
		int best = row;
		for (int i = row + 1; i < m; i++)
			if (abs(a[i][col]) > abs(a[best][col])) best = i;
		if (abs(a[best][col]) <= eps) continue;
		swap(a[best], a[row]);
		LD div = a[row][col];
		for (int j = col; j <= n; j++) a[row][j] /= div;
		for (int i = 0; i < m; i++) if (i != row) {
			LD mul = a[i][col];
			if (abs(mul) <= eps) continue;
			a[i][col] = 0;
			for (int j = col + 1; j <= n; j++)
				a[i][j] -= mul * a[row][j];
		}
		pivot[col] = row++;
	}
	for (int i = row; i < m; i++) {
		bool zero = true;
		for (int j = 0; j < n; j++)
			if (abs(a[i][j]) > eps) zero = false;
		if (zero && abs(a[i][n]) > eps) return {-1, {}};
	}
	vector<LD> x(n);
	for (int col = 0; col < n; col++)
		if (pivot[col] != -1) x[col] = a[pivot[col]][n];
	return {row, x};
}

// 模素数 p 的线性方程组；返回值含义同上
// 与浮点版本相比：非零元都能作为主元，除法改成乘逆元
pair<int, vector<int>> gauss_mod(vector<vector<int>> a, int p) {
	if (a.empty()) return {0, {}};
	auto norm = [p](long long x) {
		x %= p; return (int)(x < 0 ? x + p : x);
	};
	auto power = [p](int base, long long b) {
		int r = 1;
		for (; b; b >>= 1, base = (long long)base * base % p)
			if (b & 1) r = (long long)r * base % p;
		return r;
	};
	int m = (int)a.size(), n = (int)a[0].size() - 1, row = 0;
	for (auto& line : a) for (int& x : line) x = norm(x);
	vector<int> pivot(n, -1);
	for (int col = 0; col < n && row < m; col++) {
		int best = row;
		while (best < m && a[best][col] == 0) best++;
		if (best == m) continue;
		swap(a[best], a[row]);
		int inv = power(a[row][col], p - 2);
		for (int j = col; j <= n; j++)
			a[row][j] = (long long)a[row][j] * inv % p;
		for (int i = 0; i < m; i++) if (i != row) {
			int mul = a[i][col];
			if (mul == 0) continue;
			a[i][col] = 0;
			for (int j = col + 1; j <= n; j++)
				a[i][j] = norm(a[i][j] - (long long)mul * a[row][j]);
		}
		pivot[col] = row++;
	}
	for (int i = row; i < m; i++) {
		bool zero = true;
		for (int j = 0; j < n; j++) if (a[i][j] != 0) zero = false;
		if (zero && a[i][n] != 0) return {-1, {}};
	}
	vector<int> x(n);
	for (int col = 0; col < n; col++)
		if (pivot[col] != -1) x[col] = a[pivot[col]][n];
	return {row, x};
}
