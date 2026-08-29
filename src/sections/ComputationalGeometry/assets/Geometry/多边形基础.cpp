vp polygon_cut(cvp p, cl l) { // 保留有向直线 l 左侧（含边界）
	vp ret;
	int n = (int)p.size();
	if (!n) return ret;
	for (int i = 0; i < n; ++i) {
		P a = p[i], b = p[(i + 1) % n];
		int sa = turn(l.s, l.t, a), sb = turn(l.s, l.t, b);
		if (sa >= 0) ret.push_back(a);
		if (sa * sb < 0) ret.push_back(ll_intersection(l, {a, b}));
	}
	return ret;
}

LD polygon_signed_area2(cvp p) { // 逆时针为正，返回两倍有向面积
	LD ret = 0;
	for (int i = 0, n = (int)p.size(); i < n; ++i)
		ret += p[i] ^ p[(i + 1) % n];
	return ret;
}

LD polygon_area(cvp p) {
	return fabsl(polygon_signed_area2(p)) / 2;
}

LD polygon_perimeter(cvp p) {
	LD ret = 0;
	for (int i = 0, n = (int)p.size(); i < n; ++i)
		ret += (p[i] - p[(i + 1) % n]).len();
	return ret;
}

P polygon_centroid(cvp p) { // 非退化简单多边形
	LD area2 = polygon_signed_area2(p);
	P ret;
	for (int i = 0, n = (int)p.size(); i < n; ++i) {
		LD cross = p[i] ^ p[(i + 1) % n];
		ret = ret + (p[i] + p[(i + 1) % n]) * cross;
	}
	return ret / (3 * area2);
}

// -1: 外部，0: 边界，1: 内部；与顶点顺逆时针无关。
int point_in_polygon(cp u, cvp p) {
	bool in = false;
	for (int i = 0, n = (int)p.size(); i < n; ++i) {
		P a = p[i], b = p[(i + 1) % n];
		if (point_on_segment(u, {a, b})) return 0;
		if ((a.y > u.y) != (b.y > u.y)
			&& sgn((b - a) ^ (u - a)) == (b.y > a.y ? 1 : -1)) in = !in;
	}
	return in ? 1 : -1;
}
