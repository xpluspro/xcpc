// n*p >= c；闭约束，Range 可限制横轴开闭范围。
struct HP {
  P n; LD c;
  HP(P v={},LD b=0):n(v),c(b){}
  HP(cl l):HP((l.t-l.s).rot90(),(l.t-l.s).rot90()*l.s){}
};
const LD HINF=numeric_limits<LD>::infinity();
struct Range { LD l=-HINF,r=HINF; bool lo=0,ro=0; };
Range x_lt(LD x){return {-HINF,x,0,1};}
Range x_gt(LD x){return {x,HINF,1,0};}
bool hp_feasible(const vector<HP>& h,Range R={}) {
  struct F { LD k,b,l; }; vector<F> A,B;
  for(auto [n,c]:h) {
    LD a=n.x,b=n.y;
    if(b) (b>0?A:B).push_back({-a/fabsl(b),c/fabsl(b),0});
    else if(a>0) { if(c/a>R.l) R.l=c/a,R.lo=0; }
    else if(a<0) { if(c/a<R.r) R.r=c/a,R.ro=0; }
    else if(c>0) return 0;
  }
  if(R.l>R.r || (R.l==R.r&&(R.lo||R.ro))
      || R.l==HINF || R.r==-HINF) return 0;
  if(A.empty()||B.empty()) return 1;
  auto hull=[](vector<F> a) {
    sort(a.begin(),a.end(),[](F x,F y) {
      return x.k!=y.k ? x.k<y.k : x.b>y.b;
    });
    vector<F> q;
    for(F f:a) {
      if(!q.empty()&&f.k==q.back().k) continue;
      LD x=-HINF;
      while(!q.empty()) {
        x=(q.back().b-f.b)/(f.k-q.back().k);
        if(x>q.back().l) break;
        q.pop_back();
      }
      f.l=q.empty()?-HINF:x; q.push_back(f);
    }
    return q;
  };
  A=hull(move(A)); B=hull(move(B));
  auto good=[](F a,F b,LD x,bool open) {
    LD v=fmal(a.k,x,a.b)+fmal(b.k,x,b.b);
    LD e=32*numeric_limits<LD>::epsilon()*
        (fabsl(a.k*x)+fabsl(a.b)
        +fabsl(b.k*x)+fabsl(b.b));
    return open?v<0:v<=e;
  };
  for(size_t i=0,j=0;i<A.size()&&j<B.size();) {
    LD ni=i+1<A.size()?A[i+1].l:HINF;
    LD nj=j+1<B.size()?B[j+1].l:HINF;
    LD l=max({R.l,A[i].l,B[j].l}), r=min({R.r,ni,nj});
    LD m=A[i].k+B[j].k;
    bool lo=l==R.l&&R.lo, ro=r==R.r&&R.ro;
    if(l<r||(l==r&&!lo&&!ro)) {
      // 恒定段在内部取到最小值，用 x=0 避免代入无穷。
      if(m==0 ? good(A[i],B[j],0,0) : m>0 ?
          (l==-HINF||good(A[i],B[j],l,lo)) :
          (r== HINF||good(A[i],B[j],r,ro))) return 1;
    }
    LD x=min(ni,nj);
    if(x>=R.r) break;
    if(ni==x) ++i;
    if(nj==x) ++j;
  }
  return 0;
}
