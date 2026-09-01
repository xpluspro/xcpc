// 带修莫队

#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 5;
int n, m, B, a[N], cnt, q, pos[N], col[N], buc[N], cur, ans[N];
void add(int x) {cur += !buc[x], buc[x]++;}
void del(int x) {buc[x]--, cur -= !buc[x];}
struct query {
	int l, r, k, blkl, blkr, id;
	bool operator < (const query &v) const {
		if(blkl != v.blkl) return blkl < v.blkl;
		if(blkr != v.blkr) return blkl & 1 ? blkr > v.blkr : blkr < v.blkr;
		return blkr & 1 ? k > v.k : k < v.k;
	}
} c[N];
int main() {
	cin >> n >> m, B = pow(n, 0.67);
	for(int i = 1; i <= n; i++) cin >> a[i];
	for(int i = 1; i <= m; i++) {
		char s; int l, r;
		cin >> s >> l >> r;
		if(s == 'Q') c[++q] = {l, r, cnt, l / B, r / B, q};
		else pos[++cnt] = l, col[cnt] = r;
	}
	sort(c + 1, c + q + 1);
	for(int i = 1, l = 1, r = 0, k = 0; i <= n; i++) {
		while(r < c[i].r) add(a[++r]);
		while(l > c[i].l) add(a[--l]);
		while(r > c[i].r) del(a[r--]);
		while(l < c[i].l) del(a[l++]);
		while(k < c[i].k) {
			k++;
			if(l <= pos[k] && pos[k] <= r) del(a[pos[k]]);
			swap(col[k], a[pos[k]]);
			if(l <= pos[k] && pos[k] <= r) add(a[pos[k]]);
		}
		while(k > c[i].k) {
			if(l <= pos[k] && pos[k] <= r) del(a[pos[k]]);
			swap(col[k], a[pos[k]]);
			if(l <= pos[k] && pos[k] <= r) add(a[pos[k]]);
			k--;
		}
		ans[c[i].id] = cur;
	}
	for(int i = 1; i <= q; i++) cout << ans[i] << "\n";
	return 0;
}