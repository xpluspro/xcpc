// 求 sum_{i=1}^n f(i) mod mod，f 积性且 f(p^e)=p^e(p^e-1)。
// 预处理：sq=floor(sqrt(n))；p 含所有 <=sq 的质数；w[1..] 升序存 floor(n/i)。
// g1[j]=sum_{i=2}^{w[j]}i，g2[j]=sum_{i=2}^{w[j]}i^2（取模）。
namespace min25 {
using ll = long long;
ll n; int sq, mod;
std::vector<int> p, g1, g2, g;
std::vector<ll> w;
int md(ll x) { return (int)((x % mod + mod) % mod); }
int id(ll x) { return x <= sq ? (int)x : (int)w.size() - (int)(n / x); }

void init() { // 筛出 sum_{q<=x} q(q-1)，q 为质数
	int m = (int)w.size() - 1;
	for (int x : p) for (int j = m; j && w[j] / x >= x; --j) {
		int k = id(w[j] / x), x2 = (int)(1LL * x * x % mod);
		g1[j] = md(g1[j] - 1LL * x * md(g1[k] - (ll)g1[x - 1]));
		g2[j] = md(g2[j] - 1LL * x2 * md(g2[k] - (ll)g2[x - 1]));
	}
	g.resize(w.size());
	for (int j = 1; j <= m; ++j) g[j] = md(g2[j] - (ll)g1[j]);
}

int S(ll x, int k) {
	int pre = k ? p[k - 1] : 1;
	if (x <= pre) return 0;
	if (k == (int)p.size() || x / p[k] < p[k])
		return md(g[id(x)] - (k ? (ll)g[pre] : 0));
	int ans = S(x, k + 1), pe = p[k] % mod;
	for (ll y = x / p[k]; y; y /= p[k]) {
		int fp = md(1LL * pe * md(pe - 1LL));
		ans = md(ans + 1LL * fp * (S(y, k + 1) + 1));
		pe = (int)(1LL * pe * p[k] % mod);
	}
	return ans;
}

int solve() { init(); return md(S(n, 0) + 1LL); } // f(1)=1
} // namespace min25
