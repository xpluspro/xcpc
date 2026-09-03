// 回滚莫队 

#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;
const int B = 450;
struct dat {
	int mx, mn, val;
} stc[B + 5];
struct query {
	int l, r, blk, id;
	bool operator < (const query &v) const {
		return blk != v.blk ? blk < v.blk : r < v.r;
	}
} c[N];
int n, m, q, top, a[N], d[N];
int pre[N], suf[N], ans[N];
int add(int x, int tp) {
	if(tp) stc[++top] = {suf[a[x]], pre[a[x]], a[x]};
	if(x > suf[a[x]]) suf[a[x]] = x;
	if(x < pre[a[x]]) pre[a[x]] = x;
	return max(x - pre[a[x]], suf[a[x]] - x);
}
void Rollback() {
	while(top) {
		pre[stc[top].val] = stc[top].mn;
		suf[stc[top].val] = stc[top].mx, top--;
	}
}
int main() {
	cin >> n, memset(pre, 0x3f, sizeof(pre));
	for(int i = 1; i <= n; i++) scanf("%d", &a[i]), d[i] = a[i];
	cin >> m, sort(d + 1, d + n + 1);
	for(int i = 1; i <= n; i++) a[i] = lower_bound(d + 1, d + n + 1, a[i]) - d;
	for(int i = 1; i <= m; i++) {
		int l, r; scanf("%d %d", &l, &r);
		if(l / B == r / B) {
			for(int j = l; j <= r; j++) ans[i] = max(ans[i], add(j, 1));
			Rollback();
		} else c[++q] = {l, r, l / B, i};
	}
	sort(c + 1, c + q + 1);
	for(int i = 1, l, r, cur; i <= q; i++) {
		if(i == 1 || c[i].blk != c[i - 1].blk) {
			l = min(n + 1, c[i].blk * B + B), r = l - 1, cur = 0;
			memset(pre, 0x3f, sizeof(pre));
			memset(suf, 0, sizeof(suf));
		}
		while(r < c[i].r) cur = max(cur, add(++r, 0));
		int tmp = cur;
		while(l > c[i].l) cur = max(cur, add(--l, 1));
		ans[c[i].id] = cur, cur = tmp, Rollback();
		l = min(n + 1, c[i].blk * B + B);
	}
	for(int i = 1; i <= m; i++) printf("%d\n", ans[i]);
	return 0;
}