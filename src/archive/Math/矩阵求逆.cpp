#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N=4e2+5;
const int mod=1e9+7;

ll ksm(ll a,ll b){
	ll s=1;
	while(b){
		if(b&1)s=s*a%mod;
		a=a*a%mod,b>>=1;
	}
	return s;
}
ll inv(ll x){return ksm(x,mod-2);}

ll n,a[N][N],b[N][N];

void gauss(){
	for(int i=1;i<=n;i++){
		int p=-1;
		for(int j=i;j<=n;j++)if(a[j][i]){p=j; break;}
		if(p==-1)puts("No Solution"),exit(0);
		swap(a[i],a[p]),swap(b[i],b[p]);
		ll iv=inv(a[i][i]);
		for(int j=1;j<=n;j++)a[i][j]=a[i][j]*iv%mod,b[i][j]=b[i][j]*iv%mod;
		for(int j=i+1;j<=n;j++){
			ll c=a[j][i];
			for(int k=n;k;k--)
				b[j][k]=(b[j][k]-c*b[i][k]%mod+mod)%mod,
				a[j][k]=(a[j][k]-c*a[i][k]%mod+mod)%mod;
		}
	}
	for(int i=n-1;i;i--)
		for(int j=i+1;j<=n;j++)
			for(int k=1;k<=n;k++)
				b[i][k]=(b[i][k]-a[i][j]*b[j][k]%mod+mod)%mod;
}

int main(){
	cin>>n;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			cin>>a[i][j],b[i][j]=i==j;
	gauss();
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			cout<<b[i][j]<<(j==n?'\n':' ');
	return 0;
}