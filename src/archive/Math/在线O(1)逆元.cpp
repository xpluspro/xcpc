
//在线O(1)逆元
// from zhoukangyang 

#include<bits/stdc++.h>
#include "inv.h"
#define L(i, j, k) for(int i = (j); i <= (k); ++i)
#define R(i, j, k) for(int i = (j); i >= (k); --i)
#define ll long long 
#define vi vector < int > 
#define sz(a) ((int) (a).size())
#define ll long long 
#define ull unsigned long long
#define me(a, x) memset(a, x, sizeof(a)) 
#define eb emplace_back
using namespace std;
const int B = 1024, mod = 998244353, N = (1 << 21) + 1;
int K, pool[N * 2], *iv = pool + N;
struct M {
	int ml, dis; 
} mp[N];
int inv(int w) {
	M d = mp[w >> 10];
	return (ll) iv[w * d.ml - d.dis] * (mod + d.ml) % mod;
} 
void init(int rp) {
	K = mod / B;
	int Rlim = mod - K;
	L(fb, 1, B) {
		int cur = 0, Add = fb * B;
		for(int p = 0; p <= K; ) {
			if(cur <= K) mp[p].ml = fb;
			else if(cur > Rlim) mp[p].ml = -fb;
			else {
				int A = (Rlim - cur) / Add;
				cur += A * Add, p += A;
			}
			cur += Add, ++p;
			if(cur >= mod) cur -= mod; 
		}
	}
	int count = 0;
	L(i, 1, K) 
		if(mp[i].ml > 0) mp[i].dis = (ll) mp[i].ml * i * B / mod * mod, ++count;
		else mp[i].dis = (ll) mp[i].ml * i * B / mod * mod - mod, ++count;
	iv[1] = 1;
	L(i, 2, N - 1) iv[i] = (ll) iv[mod % i] * (mod - mod / i) % mod;
	L(i, 1, N - 1) iv[-i] = mod - iv[i];
}