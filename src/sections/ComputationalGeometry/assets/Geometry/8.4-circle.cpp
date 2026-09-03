struct C {
	P c;
	LD r;

	C(cp c_ = P(), LD r_ = 0) : c(c_), r(r_) {}
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

LD circle_segment_factor(LD t) { // t-sin(t)cos(t)，小角时避免灾难性消减
	if (fabsl(t) < 0.1L) {
		LD t2 = t * t;
		return t * t2 * (2.0L / 3 + t2 * (-2.0L / 15
			+ t2 * (4.0L / 315 + t2 * (-2.0L / 2835
			+ t2 * (4.0L / 155925 - t2 * (4.0L / 6081075))))));
	}
	return t - sinl(t) * cosl(t);
}

LD cc_intersection_area(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (sgn(d - a.r - b.r) >= 0) return 0;
	if (sgn(d - fabsl(a.r - b.r)) <= 0) {
		LD r = min(a.r, b.r);
		return pi * r * r;
	}
	// 用半角公式求两个弓形的半圆心角，所有根号内的因子在相交时均非负。
	// 这样既不需要平方差，也不会用两个大扇形减去一个大三角形。
	LD u = max(0.0L, a.r + b.r - d);
	LD v = max(0.0L, d + b.r - a.r);
	LD w = max(0.0L, d + a.r - b.r);
	LD z = max(0.0L, d + a.r + b.r);
	LD t1 = 2 * atan2l(sqrtl(u * v), sqrtl(z * w));
	LD t2 = 2 * atan2l(sqrtl(u * w), sqrtl(z * v));
	return a.r * a.r * circle_segment_factor(t1)
		+ b.r * b.r * circle_segment_factor(t2);
}

// 返回交点个数；-1 表示两圆重合，有无穷多个交点。
int cc_intersection_count(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (a.c == b.c) {
		if (sgn(a.r - b.r)) return 0;
		return !sgn(a.r) ? 1 : -1;
	}
	if (sgn(d - a.r - b.r) > 0
		|| sgn(d - fabsl(a.r - b.r)) < 0) return 0;
	if (!sgn(d - a.r - b.r) || !sgn(d - fabsl(a.r - b.r))) return 1;
	return 2;
}

vp cc_intersection(cc a, cc b) { // 仅返回有限交点；重合圆返回空集
	int cnt = cc_intersection_count(a, b);
	if (cnt <= 0) return {};
	if (a.c == b.c) return {a.c}; // 重合的零半径圆
	LD d = (a.c - b.c).len();
	P v = (b.c - a.c).unit();
	LD x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
	LD h = sqrtl(max(0.0L, a.r * a.r - x * x));
	P p = a.c + v * x;
	if (cnt == 1) return {p};
	return {p + v.rot90() * h, p - v.rot90() * h};
}

vp tangent(cp p, cc c) { // 点到圆的切点
	P d = p - c.c;
	LD d2 = d.len2(), r2 = c.r * c.r;

	// 不用 sgn：固定绝对 eps 会吞掉极靠近圆周的外点。
	if (d2 < r2) return {};
	if (d2 == r2) return {p};
	if (c.r == 0) return {c.c};

	LD k = r2 / d2;
	P foot = c.c + d * k;
	P offset = d.rot90() * (c.r * sqrtl(d2 - r2) / d2);
	return {foot + offset, foot - offset};
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
