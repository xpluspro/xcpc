struct PolygonChordSolver { // 简单多边形顶点间最长内含线段
	vp p;
	int n;
	PolygonChordSolver(vp p) : p(move(p)), n((int)this->p.size()) {}

	bool segment_inside(cp u, cp v) const {
		for (int i = 0; i < n; ++i) {
			int j = (i + 1) % n, k = (i + 2) % n;
			cp a = p[i], b = p[j], c = p[k];
			if (intersection_judge_strict({u, v}, {a, b})) return false;
			if (point_on_segment(b, {u, v})) {
				bool good = true, convex = turn(a, b, c) >= 0;
				for (cp x : {u, v}) {
					if (convex) good &= turn(a, b, x) >= 0 && turn(b, c, x) >= 0;
					else good &= !(turn(b, x, c) > 0 && turn(b, a, x) > 0);
				}
				if (!good) return false;
			}
		}
		return true;
	}

	LD ray_exit_distance(int uid, int vid) const {
		cp u = p[uid], v = p[vid];
		LD far = INF;
		for (int i = 0; i < n; ++i) {
			int j = (i + 1) % n, k = (i + 2) % n;
			cp a = p[i], b = p[j], c = p[k];
			if (two_side(a, b, {u, v})) {
				LD s1 = (b - a) ^ (u - a), s2 = (b - a) ^ (v - a);
				if (sgn(s1 - s2) && sgn(s1) != sgn(s2 - s1))
					far = min(far, (u - ll_intersection({a, b}, {u, v})).len());
			}
			if (j != uid && point_on_ray(b, {u, v})) {
				bool good = turn(a, b, c) <= 0;
				for (cp x : {u - (b - u), b + (b - u)})
					good &= !(turn(a, b, x) > 0 && turn(b, c, x) > 0);
				if (!good) far = min(far, (u - b).len());
			}
		}
		return far;
	}

	LD solve() const {
		LD ans = 0;
		for (int i = 0; i < n; ++i)
			for (int j = i + 1; j < n; ++j) {
				if (!segment_inside(p[i], p[j])) continue;
				ans = max(ans, ray_exit_distance(i, j) + ray_exit_distance(j, i)
					- (p[i] - p[j]).len());
			}
		return ans;
	}
};
