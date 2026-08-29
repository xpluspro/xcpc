LD closest_pair(vp p) { // O(n log n)，返回欧氏距离
	int n = (int)p.size();
	if (n < 2) return INF;
	sort(p.begin(), p.end());
	vector<P> tmp(n);
	function<LD(int, int)> solve = [&](int l, int r) -> LD {
		if (r - l <= 3) {
			LD ret = INF;
			for (int i = l; i < r; ++i)
				for (int j = i + 1; j < r; ++j)
					ret = min(ret, (p[i] - p[j]).len2());
			sort(p.begin() + l, p.begin() + r,
				[](cp a, cp b) { return a.y == b.y ? a.x < b.x : a.y < b.y; });
			return ret;
		}
		int m = (l + r) / 2;
		LD mid_x = p[m].x;
		LD ret = min(solve(l, m), solve(m, r));
		merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r,
			tmp.begin(), [](cp a, cp b) {
				return a.y == b.y ? a.x < b.x : a.y < b.y;
			});
		copy(tmp.begin(), tmp.begin() + r - l, p.begin() + l);
		vector<P> strip;
		for (int i = l; i < r; ++i) if ((p[i].x - mid_x) * (p[i].x - mid_x) < ret) {
			for (int j = (int)strip.size() - 1; j >= 0
				&& (p[i].y - strip[j].y) * (p[i].y - strip[j].y) < ret; --j)
				ret = min(ret, (p[i] - strip[j]).len2());
			strip.push_back(p[i]);
		}
		return ret;
	};
	return sqrtl(solve(0, n));
}
