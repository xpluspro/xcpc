// n! = a*p^b (mod p^q), p is prime; returns {a,b}, gcd(a,p)=1.
// Requires n>=0, 1<=q<=63 and p^q<=ULLONG_MAX.
// Time: $O(pq^2\frac{\log^2n}{\log p})$.
using FMU64=unsigned long long;
using FMU128=__uint128_t;
FMU64 FMMod;
FMU64 FMadd(FMU64 a,FMU64 b){return (FMU128(a)+b)%FMMod;}
FMU64 FMmul(FMU64 a,FMU64 b){return FMU128(a)*b%FMMod;}
typedef std::vector<FMU64> FMPoly;
FMPoly FMpolymul(const FMPoly &a,const FMPoly &b){
	int n=(int)a.size();FMPoly c(n,0);
	for(int i=0;i<n;++i)for(int j=0;i+j<n;++j)
		c[i+j]=FMadd(c[i+j],FMmul(a[i],b[j]));
	return c;
}FMU64 FMchoo[64][64];
FMPoly FMpolyshift(const FMPoly &a,FMU64 delta){
	int n=(int)a.size();FMPoly res(n,0);
	for(int i=0;i<n;++i){FMU64 d=1;
		for(int j=0;j<=i;++j){
			res[i-j]=FMadd(res[i-j],FMmul(FMmul(a[i],FMchoo[i][j]),d));
			d=FMmul(d,delta);}}
	return res;
}
void FMprepare(int q){
	for(int i=0;i<q;++i){FMchoo[i][0]=1;
		for(int j=1;j<=i;++j)
			FMchoo[i][j]=FMadd(FMchoo[i-1][j-1],FMchoo[i-1][j]);}
}
std::pair<FMU64,long long> FMfact(long long n,long long p,int q){
	FMU64 ans=1;
	for(long long r=1;r<p;++r){
		FMPoly x(q,0),res(q,0);
		res[0]=1;FMU64 sr=0;x[0]=(FMU64)r;FMU64 sx=0;
		if(q>1)x[1]=p,sx=1;
		long long m=n<r?0:(n-r)/p+1;
		while(m){
			if(m&1)res=FMpolymul(res,FMpolyshift(x,sr)),sr+=sx;
			m>>=1;if(!m)break;
			x=FMpolymul(x,FMpolyshift(x,sx));sx+=sx;}
		ans=FMmul(ans,res[0]);}
	long long cnt=n/p;
	if(n>=p){auto t=FMfact(n/p,p,q);
		ans=FMmul(ans,t.first);cnt+=t.second;}
	return {ans,cnt};
}
std::pair<unsigned long long,long long> fact(long long n,long long p,int q){
	assert(n>=0&&p>=2&&1<=q&&q<=63); // p must be prime
	unsigned long long pk=1,up=(unsigned long long)p;
	for(int i=0;i<q;++i){assert(pk<=~0ULL/up);pk*=up;}
	FMMod=pk;FMprepare(q);return FMfact(n,p,q);
}
