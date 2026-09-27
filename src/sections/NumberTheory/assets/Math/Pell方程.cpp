// $x^2 - n * y^2 = 1$ 最小正整数根
// n <= 0、n 为完全平方数或答案超出 LL 时返回 {-1, -1}
// $ x_{k+1} = x_0x_k + ny_0y_k $
// $ y_{k+1} = x_0y_k + y_0x_k $
pair<LL, LL> peLL(LL n) {
	if(n <= 0) return {-1, -1};
	LL a0 = (LL)sqrtl(n);
	while((__int128)(a0 + 1) * (a0 + 1) <= n) ++a0;
	while((__int128)a0 * a0 > n) --a0;
	if((__int128)a0 * a0 == n) return {-1, -1};
	LL g = 0, h = 1, a = a0;
	__int128 p0 = 0, p1 = 1, q0 = 1, q1 = 0;
	for(;;) {
		__int128 p = a * p1 + p0, q = a * q1 + q0;
		if(p > LLONG_MAX || q > LLONG_MAX) return {-1, -1};
		if(p * p - (__int128)n * q * q == 1) return {(LL)p, (LL)q};
		p0 = p1; p1 = p; q0 = q1; q1 = q;
		g = a * h - g;
		h = (n - g * g) / h;
		a = (a0 + g) / h;
	}
}
