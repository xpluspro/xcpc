#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N=600+5;

ll n,p,a[N][N],sym,prod=1;
int main(){
	cin>>n>>p;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			cin>>a[i][j],a[i][j]%=p;
	for(int i=1;i<n;i++){
		for(int j=i+1;j<=n;j++){
			while(a[i][i]){
				ll q=a[j][i]/a[i][i];
				for(int k=i;k<=n;k++)a[j][k]=(a[j][k]-q*a[i][k]%p+p)%p; 
				swap(a[i],a[j]),sym^=1;
			} swap(a[i],a[j]),sym^=1;
		}
	}
	for(int i=1;i<=n;i++)prod=(prod*a[i][i])%p;
	cout<<(sym?p-prod:prod)%p<<endl;
	return 0;
}