// 求解 x^2 同余于 n mod p 并且 p 不是2

#include <bits/stdc++.h>
using namespace std;
mt19937 rnd(1064);
int T, n, p, b, w;
struct comp {
  int a, b;
  comp operator + (comp c) {
    return {(a + c.a) % p, (b + c.b) % p};
  }
  comp operator * (comp c) {
    return {(1ll * a * c.a + 1ll * b * c.b % p * w) % p, (1ll * a * c.b + 1ll * b * c.a) % p};
  }
};
int ksm(int a, int b) {
  int s = 1;
  while(b) {
    if(b & 1) s = 1ll * s * a % p;
    a = 1ll * a * a % p, b >>= 1;
  }
  return s;
}
comp ksm(comp a, int b) {
  comp s = {1, 0};
  while(b) {
    if(b & 1) s = s * a;
    a = a * a, b >>= 1;
  }
  return s;
}
void solve() {
  cin >> n >> p;
  if(!n) return puts("0"), void();
  if(ksm(n, p - 1 >> 1) == p - 1) return puts("Hola!"), void();
  do b = rnd() % p; while(ksm(w = (1ll * b * b - n + p) % p, p - 1 >> 1) != p - 1);
  int x = ksm({b, 1}, p + 1 >> 1).a;
  cout << min(x, p - x) << " " << max(x, p - x) << "\n";
}
int main() {
  cin >> T;
  while(T--) solve();
  return 0;
}