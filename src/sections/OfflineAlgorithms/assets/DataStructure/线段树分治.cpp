// 线段树分治

const int N = 1e5 + 5;
int n, m, k, chk[N << 2], fa[N << 1], sz[N << 1];
int find(int x) {return fa[x] == x ? x : find(fa[x]);}
vpii val[N << 2];
void modify(int l, int r, int ql, int qr, int x, pii v) {
	if(ql > qr) return;
	if(ql <= l && r <= qr) return val[x].pb(v), void();
	int m = l + r >> 1;
	if(ql <= m) modify(l, m, ql, qr, x << 1, v);
	if(m < qr) modify(m + 1, r, ql, qr, x << 1 | 1, v);
}
void undo(vpii &x) {for(auto it : x) sz[it.fi] -= sz[it.se], fa[it.se] = it.se;}
void query(int l, int r, int x) {
	vpii upd;
	if(chk[x]) {
		for(auto it : val[x]) {
			int u = it.fi, v = it.se, uu = u + n, vv = v + n;
			u = find(u), v = find(v), uu = find(uu), vv = find(vv);
			if(u == v) {chk[x] = 0; break;}
			if(sz[u] < sz[vv]) swap(u, vv);
			if(sz[v] < sz[uu]) swap(v, uu);
			upd.pb(u, vv), upd.pb(v, uu);
			sz[u] += sz[vv], fa[vv] = u, sz[v] += sz[uu], fa[uu] = v;
		}
	}
	if(l == r) return puts(chk[x] ? "Yes" : "No"), undo(upd), void(); // 这里不要忘记 undo
	chk[x << 1] = chk[x << 1 | 1] = chk[x];
	int m = l + r >> 1;
	query(l, m, x << 1), query(m + 1, r, x << 1 | 1), undo(upd);
}

int main(){
	cin >> n >> m >> k, chk[1] = 1;
	for(int i = 1; i <= n << 1; i++) fa[i] = i, sz[i] = 1;
	for(int i = 1; i <= m; i++) {
		int x, y, l, r; read(x), read(y), read(l), read(r);
		modify(1, k, l + 1, r, 1, {x, y});
	} query(1, k, 1);
    return flush(), 0;
}