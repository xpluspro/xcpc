struct C {
	P c;
	LD r;

	C(cp c = P(), LD r = 0) : c(c), r(r) {}
	C(cc a) : c(a.c), r(a.r) {}
	C &operator=(cc a) { c = a.c, r = a.r; return *this; }
	C(cp a, cp b) : c((a + b) / 2), r((a - c).len()) {}
	C(cp x, cp y, cp z) { // 三点不能共线
		P p(y - x), q(z - x);
		P s(p.len2() / 2, q.len2() / 2);
		LD d = p ^ q;
		c = x + P(s ^ P(p.y, q.y), P(p.x, q.x) ^ s) / d;
		r = (c - x).len();
	}
};

bool in_circle(cp a, cc b) { return sgn((b.c - a).len() - b.r) <= 0; }

C min_circle(vp p) { // 随机增量，O(n) 期望
	if (p.empty()) return C();
	shuffle(p.begin(), p.end(), rnd);
	C ret(p[0], 0);
	for (int i = 1; i < (int)p.size(); ++i) if (!in_circle(p[i], ret)) {
		ret = C(p[i], 0);
		for (int j = 0; j < i; ++j) if (!in_circle(p[j], ret)) {
			ret = C(p[i], p[j]);
			for (int k = 0; k < j; ++k) if (!in_circle(p[k], ret))
				ret = C(p[i], p[j], p[k]);
		}
	}
	return ret;
}

vp lc_intersection(cl l, cc c) { // 直线与圆交点
	LD d = point_to_line(c.c, l);
	if (sgn(d - c.r) > 0) return {};
	P p = project_to_line(c.c, l);
	LD x = sqrtl(max(0.0L, c.r * c.r - d * d));
	if (!sgn(x)) return {p};
	P v = (l.t - l.s).unit() * x;
	return {p - v, p + v};
}

LD cc_intersection_area(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (sgn(d - a.r - b.r) >= 0) return 0;
	if (sgn(d - fabsl(a.r - b.r)) <= 0) {
		LD r = min(a.r, b.r);
		return pi * r * r;
	}
	LD x = (d * d + a.r * a.r - b.r * b.r) / (2 * d);
	LD t1 = acosl(clamp(x / a.r, -1.0L, 1.0L));
	LD t2 = acosl(clamp((d - x) / b.r, -1.0L, 1.0L));
	return a.r * a.r * t1 + b.r * b.r * t2 - d * a.r * sinl(t1);
}

vp cc_intersection(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (a.c == b.c || sgn(d - a.r - b.r) > 0
		|| sgn(d - fabsl(a.r - b.r)) < 0) return {};
	P v = (b.c - a.c).unit();
	LD x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
	LD h = sqrtl(max(0.0L, a.r * a.r - x * x));
	P p = a.c + v * x;
	if (!sgn(h)) return {p};
	return {p + v.rot90() * h, p - v.rot90() * h};
}

vp tangent(cp a, cc b) { // 点到圆的切点
	return cc_intersection(b, C(a, b.c));
}

vector<L> tangent(cc a, cc b, int f) { // f=1 外公切线，f=-1 内公切线
	P d = b.c - a.c;
	LD dr = b.r - f * a.r, d2 = d.len2();
	if (!sgn(d2) || sgn(d2 - dr * dr) < 0) return {};
	LD h = sqrtl(max(0.0L, d2 - dr * dr));
	vector<L> ret;
	for (int s : {-1, 1}) {
		P v = (d * dr + d.rot90() * (h * s)) / d2;
		ret.push_back({a.c + v * a.r, b.c + v * (f * b.r)});
		if (!sgn(h)) break;
	}
	return ret;
}
