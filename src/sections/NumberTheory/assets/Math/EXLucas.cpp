
// 求解大组合数 mod 任意数

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
void exgcd(int a, int b, int &x, int &y) {
  if(!b) return x = 1, y = 0, void();
  exgcd(b, a % b, y, x), y -= a / b * x;
}
int inv(int x, int p) {
  return exgcd(x, p, x, *new int), (x % p + p) % p;
}
ll n, m;
int p, tp, ans, curp = 1, q, qk, fc[1000005];
ll num(ll n) {
  ll s = 0;
  while(n) s += n /= q;
  return s;
}
int calc(ll n) {
  if(!n) return 1;
  return 1ll * calc(n / q) * (tp && (n / qk & 1) ? qk - 1 : 1) % qk * fc[n % qk] % qk;
} // 注意 n / q 和 n / qk!
int solve() {
  tp = (qk & 1) || qk <= 4; // 扩展 Wilson 定理.
  for(int i = fc[0] = 1; i < qk; i++) fc[i] = 1ll * fc[i - 1] * (i % q ? i : 1) % qk;
  int pw = num(n) - num(m) - num(n - m);
  int res = 1ll * calc(n) * inv(1ll * calc(m) * calc(n - m) % qk, qk) % qk;
  while(pw--) res = 1ll * res * q % qk; // 根据 Kummer 定理, 质因数在组合数中的个数为对数级别.
  return res;
}
int main() {
  cin >> n >> m >> p;
  for(int i = 2; i <= p; i++) {
    if(i * i > p) i = p;
    if(p % i == 0) {
      q = i, qk = 1;
      while(p % i == 0) qk *= i, p /= i;
      ans += 1ll * (solve() - ans % qk + qk) * inv(curp, qk) % qk * curp;
      ans %= (curp *= qk); // exCRT 合并更方便.
    }
  }
  cout << ans << endl; 
  return 0;
}