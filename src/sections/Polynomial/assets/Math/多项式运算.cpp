using poly = vector<int>;
poly poly_calc(const poly& u, const poly& v,
	function<int(int, int)> op) { // 返回长度补齐到 2 的幂
	if (u.empty() || v.empty()) return {};
	static int a[MAXN], b[MAXN], c[MAXN];
	int n = 1, need = (int)u.size() + (int)v.size() - 1;
	while (n < need) n *= 2;
	assert(n <= MAXN);
	memcpy(a, u.data(), sizeof(int) * u.size());
	fill(a + u.size(), a + n, 0);
	memcpy(b, v.data(), sizeof(int) * v.size());
	fill(b + v.size(), b + n, 0);
	ntt(a, n, 1); ntt(b, n, 1);
	for (int i = 0; i < n; i++) c[i] = op(a[i], b[i]);
	ntt(c, n, -1); return poly(c, c + n); }
poly poly_mul(const poly& u, const poly& v) { // 乘法
	return poly_calc(u, v, [](int a, int b)
		{ return (LL)a * b % p; }); } // 返回长度是两倍
poly poly_inv(const poly& a) { // 求逆，返回长度不变
	assert(!a.empty() && a[0] % p != 0);
	int size = (int)a.size(), limit = 1; while (limit < size) limit *= 2;
	poly aa = a; aa.resize(limit); poly c{qpow(a[0], p - 2)};
	for (int k = 2; k <= limit; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		c = poly_calc(b, c, [](int bi, int ci) {
			return ((2 - (LL)bi * ci) % p + p) * ci % p; });
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(size); return c; }
poly poly_sqrt(const poly& a) { // 开根，返回长度不变
	int size = (int)a.size(), limit = 1; while (limit < size) limit *= 2;
	poly aa = a; aa.resize(limit); poly c{1}; // 常数项不是 1 的话要写二次剩余
	for (int k = 2; k <= limit; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		b = poly_mul(b, poly_inv(c));
		for (int i = 0; i < k; i++) // inv_2 是 2 的逆元
			c[i] = (LL)(c[i] + b[i]) * inv_2 % p; }
	c.resize(size); return c; }
poly poly_derivative(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++) // 求导
		c[i - 1] = (LL)a[i] * i % p;
	return c; }
poly poly_integrate(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++) // 不定积分
		c[i] = (LL)a[i - 1] * inv[i] % p;
	return c; }
poly poly_ln(const poly& a) { // ln,常数项非0,返回长度不变
	auto c = poly_mul(poly_derivative(a), poly_inv(a));
	c.resize(a.size()); return poly_integrate(c); }
// exp，常数项必须是 0，返回长度不变
// 常数很大并且总代码很长, 一般可以改用分治 FFT
// 依据: 设 $G(x) = \exp F(x)$, 则 $g_i = \frac{1}{i} \sum_{k=1}^{i-1} g_{i-k} \, k \, f_k$
poly poly_exp(const poly& a) { poly c{1};
	int size = (int)a.size(), limit = 1; while (limit < size) limit *= 2;
	poly aa = a; aa.resize(limit);
	for (int k = 2; k <= limit; k *= 2) {
		c.resize(k); auto b = poly_ln(c);
		for (int i = 0; i < k; i++)
			b[i] = (aa[i] - b[i] + p) % p;
		(++b[0]) %= p; c = poly_mul(b, c);
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(size); return c; }
