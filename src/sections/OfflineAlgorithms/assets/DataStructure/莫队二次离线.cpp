// 莫队二次离线

#include <bits/stdc++.h>
using namespace std;

#define ll long long
const int N = 1e5 + 5;
int n, k, q, cnt, a[N], id[N], buc[N];
ll f[N], ans[N];
struct query {
	int l, r, blk, id;
	bool operator < (const query &v) const {
		return blk != v.blk ? blk < v.blk : blk & 1 ? r < v.r : r > v.r;
	}
} c[N];
struct dat {
	int l, r, id;
};
vector <dat> qu[N];
int main() {
	cin >> n >> q >> k;
	if(k > 14) {
		for(int i = 1; i <= q; i++) puts("0");
		exit(0);
	}
	for(int i = 0; i < 1 << 14; i++)
		if(__builtin_popcount(i) == k)
			id[cnt++] = i;
	for(int i = 1; i <= n; i++) {
		scanf("%d", &a[i]), f[i] = f[i - 1];
		for(int j = 0; j < cnt; j++) f[i] += buc[a[i] ^ id[j]];
		buc[a[i]]++;
	}
	for(int i = 1; i <= q; i++) {
		scanf("%d %d", &c[i].l, &c[i].r);
		c[i].id = i, c[i].blk = c[i].l / 333;
	}
	sort(c + 1, c + q + 1);
	for(int i = 1, l = 1, r = 0; i <= q; i++) {
		if(r < c[i].r) {
			if(l > 1) qu[l - 1].push_back({r + 1, c[i].r, -c[i].id});
			ans[c[i].id] += f[c[i].r] - f[r], r = c[i].r;
		}
		if(l > c[i].l) {
			qu[r].push_back({c[i].l, l - 1, c[i].id});
			ans[c[i].id] -= f[l - 1] - f[c[i].l - 1] + (k ? 0 : l - c[i].l), l = c[i].l;
		}
		if(r > c[i].r) {
			if(l > 1) qu[l - 1].push_back({c[i].r + 1, r, c[i].id});
			ans[c[i].id] -= f[r] - f[c[i].r], r = c[i].r;
		}
		if(l < c[i].l) {
			qu[r].push_back({l, c[i].l - 1, -c[i].id});
			ans[c[i].id] += f[c[i].l - 1] - f[l - 1] + (k ? 0 : c[i].l - l), l = c[i].l;
		}
	}
	memset(buc, 0, sizeof(buc));
	for(int i = 1; i <= n; i++) {
		for(int j = 0; j < cnt; j++) buc[a[i] ^ id[j]]++;
		for(dat it : qu[i]) {
			int id = abs(it.id), sgn = id / it.id;
			for(int p = it.l; p <= it.r; p++) ans[id] += buc[a[p]] * sgn;
		}
	}
	for(int i = 2; i <= n; i++) ans[c[i].id] += ans[c[i - 1].id];
	for(int i = 1; i <= n; i++) printf("%lld\n", ans[i]);
	return 0;
}