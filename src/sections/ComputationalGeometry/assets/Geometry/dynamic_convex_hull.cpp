struct hull { // upper hull, left to right
set<P> a; LD tot;
hull () {tot = 0;}
template<class It> LD calc(It it) {
	auto u = it == a.begin() ? a.end() : prev(it);
	auto v = next(it);
	LD ret = 0;
	if (u != a.end()) ret += *u ^ *it;
	if (v != a.end()) ret += *it ^ *v;
	if (u != a.end() && v != a.end()) ret -= *u ^ *v;
	return ret; }
void insert (P p) {
	auto same_x = a.lower_bound(P(p.x, numeric_limits<LD>::lowest()));
	if (same_x != a.end() && same_x->x == p.x) {
		if (same_x->y >= p.y) return;
		erase(same_x);
	}
	if (!a.size()) { a.insert (p); return; }
	auto it = a.lower_bound (p);
	bool out;
	if (it == a.begin()) out = (p < *it); // special case
	else if (it == a.end()) out = true;
	else out = turn(*prev(it), *it, p) > 0;
	if (!out) return;
	while (it != a.begin()) {
		auto o = prev(it);
		if (o == a.begin() || turn(*prev(o), *o, p) < 0) break;
		else erase(o); }
	while (it != a.end()) {
		auto o = next(it);
		if (o == a.end() || turn(p, *it, *o) < 0) break;
		else erase(it), it = o; }
	tot += calc(a.insert(p).first); }
template<class It> void erase(It it) { tot -= calc(it); a.erase(it); } };
