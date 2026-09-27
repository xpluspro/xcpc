
// 矩阵树定理： n^3 求解 n 点 m 边（代码中 t=0 为无向图 t=1 为有向图，其中有向图的情况被限定为以 1 为根的外向树）
				// 的边权乘积之和
				
#include <bits/stdc++.h>
using namespace std;

#define gc getchar()
#define ll long long

inline int read(){
	int x=0,sign=0; char s=gc;
	while(!isdigit(s))sign|=s=='-',s=gc;
	while(isdigit(s))x=(x<<1)+(x<<3)+(s-'0'),s=gc;
	return sign?-x:x;
}

const int N=300+5;
const int mod=1e9+7;

ll ksm(ll a,ll b){
	ll s=1;
	while(b){
		if(b&1)s=s*a%mod;
		a=a*a%mod,b>>=1;
	} return s;
}

ll n,m,t,a[N][N];
void add(ll &x,ll y){x=(x+y+mod)%mod;}

ll det(){
	ll sgn=0,prod=1;
	for(int i=2;i<n;i++){
		for(int j=i+1;j<=n;j++)if(a[j][i]&&!a[i][i])swap(a[i],a[j]),sgn^1;
		ll inv=ksm(a[i][i],mod-2);
		for(int j=i+1;j<=n;j++){
			ll tmp=inv*a[j][i]%mod;
			for(int k=i;k<=n;k++)add(a[j][k],-tmp*a[i][k]%mod);
		}
	}
	for(int i=2;i<=n;i++)prod=(prod*a[i][i])%mod;
	return (sgn?mod-prod:prod)%mod; 
}
int main(){
	cin>>n>>m>>t;
	for(int i=1;i<=m;i++){
		int u=read(),v=read(),w=read();
		add(a[u][v],-w),add(a[v][v],w);
		if(!t)add(a[v][u],-w),add(a[u][u],w);
	}
	cout<<det()<<endl;
	return 0;
}