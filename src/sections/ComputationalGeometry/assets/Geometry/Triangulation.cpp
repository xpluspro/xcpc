vector<array<int, 3>> ear_clipping(cvp p) { // 简单多边形，O(n^2)
	int n = (int)p.size();
	if (n < 3) return {};
	vector<int> id(n);
	iota(id.begin(), id.end(), 0);
	if (polygon_signed_area2(p) < 0) reverse(id.begin(), id.end());
	vector<array<int, 3>> ret;
	for (bool changed = true; changed && id.size() > 3; ) {
		changed = false;
		for (int i = 0, m = (int)id.size(); i < m; ++i) {
			int a = id[(i + m - 1) % m], b = id[i], c = id[(i + 1) % m];
			if (!turn(p[a], p[b], p[c]) && point_on_segment(p[b], {p[a], p[c]})) {
				id.erase(id.begin() + i), changed = true;
				break;
			}
		}
	}

	auto in_triangle = [&](cp q, cp a, cp b, cp c) {
		return turn(a, b, q) >= 0 && turn(b, c, q) >= 0
			&& turn(c, a, q) >= 0;
	};
	while (id.size() > 3) {
		bool found = false;
		for (int i = 0, m = (int)id.size(); i < m; ++i) {
			int a = id[(i + m - 1) % m], b = id[i], c = id[(i + 1) % m];
			if (turn(p[a], p[b], p[c]) <= 0) continue;
			bool empty = true;
			for (int x : id) if (x != a && x != b && x != c
				&& in_triangle(p[x], p[a], p[b], p[c])) {
				empty = false;
				break;
			}
			if (!empty) continue;
			ret.push_back({a, b, c});
			id.erase(id.begin() + i);
			found = true;
			break;
		}
		if (!found) return {}; // 自交、重复点或退化输入
	}
	ret.push_back({id[0], id[1], id[2]});
	return ret;
}
