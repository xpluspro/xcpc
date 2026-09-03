// 长链剖分实现 O(1) 树上 k 级祖先

const int N = 5e5 + 5;
const int K = 19;
int cnt, hd[N], nxt[N], to[N]; ll ans;
void add(int u, int v) {nxt[++cnt] = hd[u], hd[u] = cnt, to[cnt] = v;}
int n, q, root, f[N][K], h[N], dep[N], mxd[N], son[N], top[N];
vector <int> up[N], down[N];
void init(int id) {
	dep[id] = dep[f[id][0]] + 1, mxd[id] = 1;
	for(int i = 1; i < K; i++) f[id][i] = f[f[id][i - 1]][i - 1];
	for(int i = hd[id]; i; i = nxt[i]) {
		init(to[i]), mxd[id] = max(mxd[id], mxd[to[i]] + 1);
		if(mxd[son[id]] < mxd[to[i]]) son[id] = to[i];
	}
}
void dfs(int id, int t) {
	if(id == t) up[id].resize(mxd[id]), down[id].resize(mxd[id]);
	down[t][dep[id] - dep[t]] = id, top[id] = t;
	if(son[id]) dfs(son[id], t);
	for(int i = hd[id]; i; i = nxt[i]) if(to[i] != son[id]) dfs(to[i], to[i]);
}
int main() {
	cin >> n >> q >> s;
	for(int i = 1; i <= n; i++) !(f[i][0] = read()) ? (root = i, void()) : add(f[i][0], i);
	init(root), dfs(root, root);
	for(int i = 1; i <= n; i++) if(top[i] == i)
		for(int j = 0, cur = i; j < mxd[i] && cur; j++, cur = f[cur][0]) up[i][j] = cur;
	for(int i = 1; i <= n; i++) for(int j = 0; j < K; j++) if(i >> j & 1) h[i] = j;
	for(int i = 1, las = 0; i <= q; i++) {
		int x = (get(s) ^ las) % n + 1, k = (get(s) ^ las) % dep[x];
		x = !k ? x : f[x][h[k]], k -= (k == 0 ? 0 : (1 << h[k]));
		int t = top[x], res;
		if(dep[x] - k >= dep[t]) res = down[t][dep[x] - k - dep[t]];
		else res = up[t][dep[t] - (dep[x] - k)];
		ans ^= 1ll * i * (las = res);
	} cout << ans << endl;
	return 0;
}