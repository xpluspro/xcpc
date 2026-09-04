#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll n, a, b, c, d;
void exgcd(ll a, ll b, ll &x, ll &y) {
  if(!b) return x = 1, y = 0, void();
  exgcd(b, a % b, y, x), y -= a / b * x;
}
int main() {
  cin >> n >> a >> b;
  for(int i = 1; i < n; i++) {
    cin >> c >> d;
    ll e = __gcd(a, c), f = (d - b % c + c) % c, inv;
    c /= e, f /= e;
    exgcd(a / e, c, inv, *new ll), inv = (inv % c + c) % c;
    b += (__int128)f * inv % c * a, a *= c, b %= a;
  }
  cout << b << endl;
  return 0;
}