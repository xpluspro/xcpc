struct ConvexQuery { // a 为逆时针严格凸包
	vp a;
	int n;
	ConvexQuery(vp points = {}) : a(move(points)), n((int)a.size()) {}

	P at(int i) const { // 空凸包没有合法下标
		if (!n) throw out_of_range("ConvexQuery::at() called on an empty polygon");
		i %= n;
		if (i < 0) i += n;
		return a[i];
	}
	pair<vp, vp> monotone_chains() const { // 返回下链（最左到最右）、上链（最右到最左）
		if (!n) return {};
		int left = 0, right = 0;
		for (int i = 1; i < n; ++i) {
			if (a[i] < a[left]) left = i;
			if (a[right] < a[i]) right = i;
		}
		vp lower{a[left]}, upper{a[right]};
		for (int i = left; i != right; ) {
			i = (i + 1) % n;
			lower.push_back(a[i]);
		}
		for (int i = right; i != left; ) {
			i = (i + 1) % n;
			upper.push_back(a[i]);
		}
		return {lower, upper};
	}
	bool inside(cp u) const { // 含边界
		if (!n) return false;
		if (n == 1) return u == a[0];
		if (n == 2) return point_on_segment(u, {a[0], a[1]});
		if (turn(a[0], a[1], u) < 0 || turn(a[0], a[n - 1], u) > 0)
			return false;
		int l = 1, r = n - 1;
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (turn(a[0], a[m], u) >= 0) l = m;
			else r = m;
		}
		return turn(a[l], a[(l + 1) % n], u) >= 0;
	}

	template<class F> int extreme(F better) const {
		int l = 0, r = n - 1, d = 1;
		if (better(a[r], a[l])) swap(l, r), d = -1;
		while (d * (r - l) > 1) {
			int m = (l + r) / 2;
			if (better(a[m], a[l]) && better(a[m], a[m - d])) l = m;
			else r = m;
		}
		return l;
	}

	pair<int, int> tangents(cp u) const { // u 严格在凸包外，n >= 3
		return {
			extreme([&](cp x, cp y) { return turn(u, y, x) > 0; }),
			extreme([&](cp x, cp y) { return turn(u, x, y) > 0; })};
	}

	int edge_crossing(cp u, cp v, int l, int r) const {
		int side = turn(u, v, at(l));
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (side == turn(u, v, at(m))) l = m;
			else r = m;
		}
		return l % n;
	}
	bool line_crossings(cp u, cp v, int &i, int &j) const {
		int p0 = extreme([&](cp x, cp y) {
			return ((v - u) ^ (x - u)) < ((v - u) ^ (y - u));
		});
		int p1 = extreme([&](cp x, cp y) {
			return ((v - u) ^ (x - u)) > ((v - u) ^ (y - u));
		});
		if (turn(u, v, a[p0]) * turn(u, v, a[p1]) >= 0) return false;
		if (p0 > p1) swap(p0, p1);
		i = edge_crossing(u, v, p0, p1);
		j = edge_crossing(u, v, p1, p0 + n);
		return true;
	}

	LD chain_distance(cp u, int l, int r) const {
		if (l > r) r += n;
		int side = sgn((u - at(l)) * (at(l + 1) - at(l)));
		LD ret = point_to_segment(u, {at(l), at(l + 1)});
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (side == sgn((u - at(m)) * (at(m + 1) - at(m)))) l = m;
			else r = m;
		}
		return min(ret, point_to_segment(u, {at(l), at(l + 1)}));
	}
	LD distance(cp u) const {
		if (inside(u)) return 0;
		if (n == 1) return (u - a[0]).len();
		if (n == 2) return point_to_segment(u, {a[0], a[1]});
		auto [x, y] = tangents(u);
		return min(chain_distance(u, x, y), chain_distance(u, y, x));
	}
};
