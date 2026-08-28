LD angle (cp u, cp v) {
	return 2 * asinl(clamp((u.unit() - v.unit()).len() / 2, 0.0L, 1.0L)); }
LD circle_edge_area2(cp s, cp t, LD r) { // 2 * area
	LD theta = angle(s, t);
	LD d = point_to_segment({0, 0}, {s, t});
	if (sgn(d - r) >= 0) return theta * r * r;
	auto q = lc_intersection({s, t}, C({0, 0}, r));
	if (q.size() < 2) return theta * r * r;
	P lo = sgn(s ^ q[0]) >= 0 ? q[0] : s;
	P hi = sgn(q[1] ^ t) >= 0 ? q[1] : t;
	return (lo ^ hi) + (theta - angle(lo, hi)) * r * r; }
LD polygon_circle_intersection_area(cvp p, cc c) {
	LD ret = 0;
	for (int i = 0; i < (int) p.size (); ++i) {
		auto u = p[i] - c.c;
		auto v = p[(i + 1) % p.size()] - c.c;
		int s = sgn(u ^ v);
		if	  (s > 0) ret += circle_edge_area2(u, v, c.r);
		else if (s < 0) ret -= circle_edge_area2(v, u, c.r);
	} return fabsl(ret) / 2; } // ret 在 p 逆时针时为正
