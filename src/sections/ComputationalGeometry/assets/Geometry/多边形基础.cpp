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
