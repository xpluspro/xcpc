// 排列元素为 1..n，字典序排名从 0 开始；unsigned long long 下要求 n <= 20。
namespace cantor {
using ULL = unsigned long long;

struct BIT {
	int n;
	std::vector<int> t;
	BIT(int size) : n(size), t(size + 1) {}
	void add(int x, int v) {
		for (; x <= n; x += x & -x) t[x] += v;
	}
	int sum(int x) const {
		int ans = 0;
		for (; x; x -= x & -x) ans += t[x];
		return ans;
	}
	// 返回第 k 小（k 从 0 开始）的仍未使用元素。
	int kth(int k) const {
		int x = 0;
		for (int d = 1 << (31 - __builtin_clz(n)); d; d >>= 1) {
			int y = x + d;
			if (y <= n && t[y] <= k) x = y, k -= t[y];
		}
		return x + 1;
	}
};

std::vector<ULL> factorial(int n) {
	std::vector<ULL> fac(n + 1, 1);
	for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
	return fac;
}

ULL encode(const std::vector<int>& p) {
	int n = (int)p.size();
	auto fac = factorial(n);
	BIT bit(n);
	for (int x = 1; x <= n; ++x) bit.add(x, 1);
	ULL rank = 0;
	for (int i = 0; i < n; ++i) {
		rank += (ULL)bit.sum(p[i] - 1) * fac[n - 1 - i];
		bit.add(p[i], -1);
	}
	return rank;
}

// 要求 0 <= rank < n!。
std::vector<int> decode(int n, ULL rank) {
	auto fac = factorial(n);
	BIT bit(n);
	for (int x = 1; x <= n; ++x) bit.add(x, 1);
	std::vector<int> p(n);
	for (int i = 0; i < n; ++i) {
		ULL block = fac[n - 1 - i];
		int k = (int)(rank / block);
		rank %= block;
		p[i] = bit.kth(k);
		bit.add(p[i], -1);
	}
	return p;
}
} // namespace cantor
