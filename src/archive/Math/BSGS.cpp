
// a^x 同余于 b mod p 要求 (a,p) == 1 ! 

#include <bits/stdc++.h>
using namespace std;
int BSGS(int a, int b, int p) {
  int A = 1, B = sqrt(p) + 1;
  a %= p, b %= p;
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
int main() {
  int p, b, n;
  cin >> p >> b >> n;
  int ans = BSGS(b,n,p);
  if(~ans) cout << ans << "\n";
  else puts("no solution");
  return 0;
}