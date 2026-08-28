int circle_count;
C circles[MAXN];
LD coverage_area[MAXN]; // coverage_area[k]：恰被 k 个圆覆盖的面积

struct Event {
	LD ang;
	int delta;
	bool operator<(const Event &o) const { return ang < o.ang; }
};

bool same_circle(cc a, cc b) {
	return a.c == b.c && !sgn(a.r - b.r);
}
bool contains(cc a, cc b) { // 圆 a 包含圆 b
	return sgn(a.r - b.r - (a.c - b.c).len()) >= 0;
}

void add_interval(LD l, LD r, vector<Event> &e, int &base) {
	const LD tau = 2 * pi;
	while (l < 0) l += tau, r += tau;
	while (l >= tau) l -= tau, r -= tau;
	if (r < tau) e.push_back({l, 1}), e.push_back({r, -1});
	else {
		++base;
		e.push_back({r - tau, -1});
		e.push_back({l, 1});
	}
}

void circle_union() {
	fill(coverage_area, coverage_area + circle_count + 2, 0);
	for (int i = 0; i < circle_count; ++i) {
		bool duplicate = false;
		for (int j = 0; j < i; ++j)
			if (same_circle(circles[i], circles[j])) duplicate = true;
		if (duplicate) continue;

		int cnt = 1;
		vector<Event> e;
		for (int j = 0; j < circle_count; ++j) if (i != j) {
			if (same_circle(circles[i], circles[j])) { ++cnt; continue; }
			if (contains(circles[j], circles[i])) { ++cnt; continue; }
			if (contains(circles[i], circles[j])) continue;
			LD d = (circles[j].c - circles[i].c).len();
			if (sgn(d - circles[i].r - circles[j].r) >= 0) continue;
			LD a = atan2l(circles[j].c.y - circles[i].c.y,
				circles[j].c.x - circles[i].c.x);
			LD b = acosl(clamp((circles[i].r * circles[i].r + d * d
				- circles[j].r * circles[j].r) / (2 * circles[i].r * d),
				-1.0L, 1.0L));
			add_interval(a - b, a + b, e, cnt);
		}
		e.push_back({0, 0});
		e.push_back({2 * pi, 0});
		sort(e.begin(), e.end());
		for (int j = 0; j + 1 < (int)e.size(); ++j) {
			cnt += e[j].delta;
			LD l = e[j].ang, r = e[j + 1].ang, d = r - l;
			P p = circles[i].c + P(cosl(l), sinl(l)) * circles[i].r;
			P q = circles[i].c + P(cosl(r), sinl(r)) * circles[i].r;
			coverage_area[cnt] += (p ^ q) / 2
				+ circles[i].r * circles[i].r * (d - sinl(d)) / 2;
		}
	}
}
