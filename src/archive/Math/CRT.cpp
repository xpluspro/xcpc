#include <bits/stdc++.h>
using namespace std;
constexpr int N = 10 + 5;
int n, a[N], b[N];
long long M = 1, ans;
void exgcd(int a, int b, int &x, int &y) {
	if(!b) return x = 1, y = 0, void();
	exgcd(b, a % b, y, x), y -= a / b * x;
}
int inv(int x, int p) {
  return exgcd(x, p, x, *new int), (x % p + p) % p;
}
int main() {
	cin >> n;
	for(int i = 1; i <= n; i++) cin >> a[i] >> b[i], M *= a[i];
	for(int i = 1; i <= n; i++) ans = (ans + (__int128)b[i] * (M / a[i]) * inv(M / a[i] % a[i], a[i])) % M;
	cout << ans << endl;
	return 0;
}