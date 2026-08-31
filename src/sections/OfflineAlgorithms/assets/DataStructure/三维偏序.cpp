// 三维偏序

#include<bits/stdc++.h>
using namespace std;

const int N=1e5+5;

int n,m,k,f[N];
struct pt{
	int a,b,c,ans,cnt;
}d[N],u[N];
bool cmp1(pt a,pt b){return a.a!=b.a?a.a<b.a:a.b!=b.b?a.b<b.b:a.c<b.c;}
bool cmp2(pt a,pt b){return a.b!=b.b?a.b<b.b:a.c<b.c;}

int c[N<<1];
void add(int x,int v){while(x<=k)c[x]+=v,x+=x&-x;}
int query(int x){int s=0; while(x)s+=c[x],x-=x&-x; return s;}

void solve(int l,int r){
	if(l==r)return;
	int m=l+r>>1,le=l;
	solve(l,m),solve(m+1,r);
	sort(u+l,u+m+1,cmp2),sort(u+m+1,u+r+1,cmp2);
	for(int i=m+1;i<=r;i++){
		while(le<=m&&u[le].b<=u[i].b)add(u[le].c,u[le].cnt),le++;
		u[i].ans+=query(u[i].c);
	} for(int i=l;i<le;i++)add(u[i].c,-u[i].cnt);
}

int main(){
	cin>>n>>k;
	for(int i=1;i<=n;i++)d[i].a=read(),d[i].b=read(),d[i].c=read();
	sort(d+1,d+n+1,cmp1);
	for(int i=1;i<=n;i++){
		if(d[i].a!=d[i-1].a||d[i].b!=d[i-1].b||d[i].c!=d[i-1].c)u[++m]=d[i];
		u[m].cnt++;
	} solve(1,m);
	for(int i=1;i<=m;i++)f[u[i].ans+u[i].cnt-1]+=u[i].cnt;
	for(int i=1;i<=n;i++)print(f[i-1]),pc('\n');
	return flush(),0;
}