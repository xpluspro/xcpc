P incenter(cp a, cp b, cp c) {
	LD p = (a-b).len() + (b-c).len() + (c-a).len();
	return (a*(b-c).len() + b*(c-a).len() + c*(a-b).len()) / p; }
P circumcenter(cp a, cp b, cp c) { return C(a, b, c).c; }
P orthocenter(cp a, cp b, cp c) {
	return a + b + c - circumcenter(a, b, c) * 2; }
P fermat_point(cp a, cp b, cp c) {
	if (a == b) return a;
	if (b == c) return b;
	if (c == a) return c;
	LD ab = (a-b).len(), bc = (b-c).len(), ca = (c-a).len();
	LD cosa = ((b-a)*(c-a)) / ab / ca;
	LD cosb = ((a-b)*(c-b)) / ab / bc;
	LD cosc = ((b-c)*(a-c)) / ca / bc;
	LD sq3 = pi / 3; P mid;
	if (sgn (cosa + 0.5) < 0) mid = a;
	else if (sgn (cosb + 0.5) < 0) mid = b;
	else if (sgn (cosc + 0.5) < 0) mid = c;
	else if (sgn((b-a)^(c-a)) < 0)
		mid = ll_intersection({a, b+(c-b).rot(sq3)}, {b, c+(a-c).rot(sq3)});
	else mid = ll_intersection({a, c+(b-c).rot(sq3)}, {c, b+(a-b).rot(sq3)});
	return mid; } // minimize(|A-x|+|B-x|+|C-x|)
