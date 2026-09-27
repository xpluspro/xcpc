
// n^0.25 * log n * log n 

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
ll rd(ll l, ll r) {return rnd() % (r - l + 1) + l;}
ll ksm(ll a, ll b, ll p) {
  ll s = 1;
  while(b) {
    if(b & 1) s = (__int128) s * a % p;
    a = (__int128) a * a % p, b >>= 1;
  }
  return s;
}
bool Miller(ll n) {
  if(n < 3 || n % 2 == 0) return n == 2;
  ll r = n - 1, d = 0;
  while(r & 1 ^ 1) r >>= 1, d++;
  for(int _ = 0; _ < 10; _++) {
    ll a = rd(2, n - 1), v = ksm(a, r, n);
    if(v == 1) continue;
    for(int i = 0; i <= d; i++) {
      if(i == d) return 0;
      if(v == n - 1) break;
      v = (__int128) v * v % n;
    }
  }
  return 1;
}
ll Pollard(ll n) {
  ll c = rd(1, n - 1), s = c, t = 0;
  auto f = [&](ll x) {return ((__int128) x * x + c) % n;};
  ll acc = 0, prod = 1, d, limit = 1;
  while(s != t) {
    prod = (__int128) prod * abs(s - t) % n;
    if(++acc == limit) {
      if((d = __gcd(prod, n)) > 1) return d;
      acc = 0, limit = min(127ll, limit << 1);
    }
    s = f(f(s)), t = f(t);
  }
  if((d = __gcd(prod, n)) > 1) return d;
  return n;
}
ll mxp(ll n) {
  if(Miller(n)) return n;
  if(n == 1) return 1;
  ll d = Pollard(n);
  while(d == n) d = Pollard(n);
  while(n % d == 0) n /= d;
  return max(mxp(d), mxp(n));
}
int main() {
  ios::sync_with_stdio(0);
  ll T, n;
  cin >> T;
  while(T--) {
    cin >> n;
    if(Miller(n)) cout << "Prime\n";
    else cout << mxp(n) << "\n";
  }
  return 0;
}