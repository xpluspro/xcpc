mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
ll rd(ll l, ll r) {return rnd() % (r - l + 1) + l;}
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