vector <LL> solve(LL r) {
	auto isqrt = [](LL n) {
		LL x = (LL)sqrtl((LD)n);
		while ((__int128)(x + 1) * (x + 1) <= n) ++x;
		while ((__int128)x * x > n) --x;
		return x;
	};
	vector <LL> ret; // non-negative Y pos
	ret.push_back(0);
	LL l = 2 * r, s = isqrt(l);
	for (LL d=1; d<=s; d++) if (l%d==0) {
		LL lim = isqrt(l/(2*d));
		for (LL a = 1; a <= lim; a++) {
			LL b = isqrt(l/d-a*a);
			if (a*a+b*b==l/d && __gcd(a,b)==1 && a!=b)
				ret.push_back(d*a*b); 
		} if (d*d==l) break;
		lim = isqrt(d/2);
		for (LL a=1; a<=lim; a++) {
			LL b = isqrt(d-a*a);
			if (a*a+b*b==d && __gcd(a,b)==1 && a!=b)
				ret.push_back(l/d*a*b);
	} }
	ret.push_back(r);
	sort(ret.begin(), ret.end());
	ret.erase(unique(ret.begin(), ret.end()), ret.end());
	return ret; }
