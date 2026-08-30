struct IL : P { // ax + by + c >= 0，适合整数半平面
	LD z;
	IL(LD a = 0, LD b = 0, LD c = 0) : P(a, b), z(c) {}
	IL(cp a, cp b) : P((b - a).rot90()), z(a ^ b) {}
	LD operator()(cp a) const { return a * static_cast<const P &>(*this) + z; }
};
using cil = const IL &;

P il_intersection(cil u, cil v) {
	return P(P(u.z, u.y) ^ P(v.z, v.y),
		P(u.x, u.z) ^ P(v.x, v.z)) / -(static_cast<const P &>(u)
		^ static_cast<const P &>(v));
}
LD signed_distance(cil l, cp x = P()) { return l(x) / l.len(); }
bool il_parallel(cil a, cil b) { return !sgn(a ^ b); }
LD det3(cil a, cil b, cil c) {
	return (a ^ b) * c.z + (b ^ c) * a.z + (c ^ a) * b.z;
}
int check(cil a, cil b, cil c) {
	return sgn(det3(b, c, a)) * sgn(b ^ c);
}
bool il_turn_left(cil a, cil b, cil c) { return check(a, b, c) > 0; }
bool il_cmp(cil a, cil b) {
	if (il_parallel(a, b) && a * b > 0)
		return signed_distance(a) < signed_distance(b);
	return half(a) == half(b) ? sgn(a ^ b) > 0 : half(b) > 0;
}

IL perpendicular(cil l) { return {l.y, -l.x, 0}; }
IL parallel_through(cil l, cp o) { return {l.x, l.y, l.z - l(o)}; }
P project(cp x, cil l) { return x - static_cast<const P &>(l) * (l(x) / l.len2()); }
P reflect(cp x, cil l) { return x - static_cast<const P &>(l) * (2 * l(x) / l.len2()); }
bool il_perpendicular(cil a, cil b) { return !sgn(a * b); }
LD triangle_area(cil a, cil b, cil c) {
	LD d = det3(a, b, c);
	return d * d / (2 * (a ^ b) * (b ^ c) * (c ^ a));
}

vector<IL> cut_integral_hpi(const vector<IL> &o, IL l) {
	vector<IL> ret;
	int n = (int)o.size();
	for (int i = 0; i < n; ++i) {
		cil u = o[i], v = o[(i + 1) % n], w = o[(i + 2) % n];
		int va = check(l, u, v), vb = check(l, v, w);
		if (va > 0 || vb > 0 || (!va && !vb)) ret.push_back(v);
		if (va >= 0 && vb < 0) ret.push_back(l);
	}
	return ret;
}
