#include <bits/stdc++.h>
using namespace std;
int a, p, b;
void exgcd(int a, int b, int &x, int &y) {
	if(!b) return x = 1, y = 0, void();
	exgcd(b, a % b, y, x), y -= a / b * x;
}
int inv(int x, int p) {
  exgcd(x, p, x, *new int);
  return (x % p + p) % p;
}
int BSGS(int a, int b, int p) {
	a %= p, b %= p;
	int A = 1, B = sqrt(p) + 1;
	unordered_map<int, int> mp;
	for(int i = 1; i <= B; i++) {
    A = 1ll * A * a % p;
    mp[1ll * A * b % p] = i;
  }
	for(int i = 1, AA = A; i <= B; i++) {
    if(mp.find(AA) != mp.end()) {
      return i * B - mp[AA];
    }
    AA = 1ll * AA * A % p;
  }
	return -1;
}
int exBSGS(int a, int b, int p) {
	int d = __gcd(a, p), k = 0;
	a %= p, b %= p;
	while(d > 1) {
		if(b == 1) return k;
		if(b % d) return -1;
		p /= d;
    b = 1ll * b / d * inv(a / d, p) % p;
		d = __gcd(p, a %= p), k++;
	}
	int ans = BSGS(a, b, p);
	return ~ans ? ans + k : -1;
}
int main() {
	cin >> a >> p >> b;
	while(a) {
		int ans = exBSGS(a, b, p);
		if(~ans) cout << ans << "\n";
		else puts("No Solution");
		cin >> a >> p >> b;
	}
	return 0;
}