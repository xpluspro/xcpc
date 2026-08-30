LD convex_diameter(cvp p) { // p 为逆时针凸包，无重复末点
	int n = (int)p.size();
	if (n < 2) return 0;
	if (n == 2) return (p[0] - p[1]).len();
	LD ans2 = 0;
	for (int i = 0, j = 1; i < n; ++i) {
		int ni = (i + 1) % n;
		while (fabsl((p[ni] - p[i]) ^ (p[(j + 1) % n] - p[i]))
			> fabsl((p[ni] - p[i]) ^ (p[j] - p[i])) + eps)
			j = (j + 1) % n;
		ans2 = max({ans2, (p[i] - p[j]).len2(), (p[ni] - p[j]).len2()});
	}
	return sqrtl(ans2);
}

vp remove_repeated_convex_vertices(cvp p) {
	vp q;
	for (cp x : p) if (q.empty() || x != q.back()) q.push_back(x);
	if (q.size() > 1 && q.front() == q.back()) q.pop_back();
	return q;
}

LD convex_min_width(cvp input) { // input 为逆时针凸包，允许相邻重复点
	vp p = remove_repeated_convex_vertices(input);
	int n = (int)p.size();
	if (n < 3) return 0;
	LD ans = INF;
	for (int i = 0, j = 1; i < n; ++i) {
		int ni = (i + 1) % n;
		while (((p[ni] - p[i]) ^ (p[(j + 1) % n] - p[i]))
			> ((p[ni] - p[i]) ^ (p[j] - p[i])) + eps)
			j = (j + 1) % n;
		ans = min(ans, ((p[ni] - p[i]) ^ (p[j] - p[i]))
			/ (p[ni] - p[i]).len());
	}
	return ans;
}

struct MinRectangle {
	LD area = 0;
	array<P, 4> p{};
};

MinRectangle min_area_rectangle(cvp input) { // input 为逆时针凸包，允许相邻重复点
	vp p = remove_repeated_convex_vertices(input);
	int n = (int)p.size();
	if (!n) return {};
	if (n == 1) return {0, {p[0], p[0], p[0], p[0]}};
	MinRectangle ans{INF, {}};
	P first_u = (p[1] - p[0]).unit(), first_v = first_u.rot90();
	int high = 0, right = 0, left = 0;
	for (int j = 1; j < n; ++j) {
		if (p[j] * first_v > p[high] * first_v) high = j;
		if (p[j] * first_u > p[right] * first_u) right = j;
		if (p[j] * first_u < p[left] * first_u) left = j;
	}
	for (int i = 0; i < n; ++i) {
		P u = (p[(i + 1) % n] - p[i]).unit(), v = u.rot90();
		while (p[(high + 1) % n] * v > p[high] * v + eps)
			high = (high + 1) % n;
		while (p[(right + 1) % n] * u > p[right] * u + eps)
			right = (right + 1) % n;
		while (p[(left + 1) % n] * u < p[left] * u - eps)
			left = (left + 1) % n;
		LD min_u = p[left] * u, max_u = p[right] * u;
		LD min_v = p[i] * v, max_v = p[high] * v;
		LD area = (max_u - min_u) * (max_v - min_v);
		if (area < ans.area) ans = {area, {
			u * min_u + v * min_v, u * max_u + v * min_v,
			u * max_u + v * max_v, u * min_u + v * max_v}};
	}
	return ans;
}
