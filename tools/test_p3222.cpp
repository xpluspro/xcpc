// P3222 仅作独立验收题，正式板子没有题目专用逻辑。
#include <bits/stdc++.h>
using namespace std;
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.1-declarations.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp"
#include "../src/sections/ComputationalGeometry/assets/Geometry/halfplane_feasible_closed.cpp"
using Target = array<long long,3>;
bool check_prefix(const vector<Target>& targets, int n) {
  vector<HP> h; h.reserve(2*n);
  for (int i=0;i<n;++i) {
    auto [x,y1,y2]=targets[i];
    h.emplace_back(P{(LD)x*x,(LD)x},(LD)y1);
    h.emplace_back(P{-(LD)x*x,-(LD)x},-(LD)y2);
  }
  return hp_feasible(h,x_lt(0));
}
int solve(const vector<Target>& targets) {
  int l=0,r=(int)targets.size();
  while (l<r) {
    int mid=(l+r+1)/2;
    if(check_prefix(targets,mid)) l=mid; else r=mid-1;
  }
  return l;
}
// 独立 oracle：消去 b，逐对 lower_i <= upper_j 得到 a 的有理数区间。
// 系数为整数；分数比较用连分数，避免交叉乘法超过 __int128。
using I = __int128_t;
struct Fraction { I n,d; };
bool positive_less(I a,I b,I c,I d) {
  bool reverse=false;
  while (true) {
    I q=a/b, r=c/d;
    if(q!=r) return reverse ? q>r : q<r;
    a%=b; c%=d;
    if(!a || !c) return reverse ? c==0 && a!=0 : a==0 && c!=0;
    swap(a,b); swap(c,d); reverse=!reverse;
  }
}
bool less_fraction(Fraction a,Fraction b) {
  if((a.n<0)!=(b.n<0)) return a.n<0;
  if(a.n<0) return positive_less(-b.n,b.d,-a.n,a.d);
  return positive_less(a.n,a.d,b.n,b.d);
}
bool exact_prefix(const vector<Target>& targets,int n) {
  optional<Fraction> left;
  Fraction right{0,1}; bool right_open=true;
  for(int i=0;i<n;++i) for(int j=0;j<n;++j) {
    I xi=targets[i][0], xj=targets[j][0];
    I k=(xj-xi)*xi*xj, c=(I)targets[j][2]*xi-(I)targets[i][1]*xj;
    if(!k) { if(c<0) return false; continue; }
    if(k>0) {
      Fraction bound{c,k};
      if(less_fraction(bound,right)) right=bound,right_open=false;
    } else {
      Fraction bound{-c,-k};
      if(!left || less_fraction(*left,bound)) left=bound;
    }
  }
  if(!left) return true;
  return right_open ? less_fraction(*left,right) : !less_fraction(right,*left);
}
int main(int argc,char** argv) {
  // 可将真实题目输入交给此程序，或无参数执行回归测试。
  if(argc==2 && string(argv[1])=="--solve") {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin>>n; vector<Target> a(n);
    for(auto& t:a) cin>>t[0]>>t[1]>>t[2];
    cout<<solve(a)<<'\n'; return 0;
  }
  assert(solve({{1,1,2},{2,4,5}})==1); // 只有 a=0 才可行，必须排除
  assert(solve({{1,1,2},{2,1,2}})==2);
  assert(solve({{1,1,1},{2,2,2}})==1); // 共线零宽靶子
  assert(solve({{1,2,2},{2,2,2}})==2); // 真正的 a=-1,b=3 单点
  // 校验连分数比较器，与不会溢出的交叉乘法对照。
  mt19937_64 gen(20260911);
  for(int t=0;t<10000;++t) {
    Fraction a{(I)(gen()%2001)-1000,(I)(gen()%1000)+1};
    Fraction b{(I)(gen()%2001)-1000,(I)(gen()%1000)+1};
    assert(less_fraction(a,b)==(a.n*b.d<b.n*a.d));
  }
  for(int trial=0;trial<3000;++trial) {
    int n=1+gen()%8; vector<Target> a;
    for(int i=0;i<n;++i) {
      long long x=trial%3==0 ? 1000000000-gen()%100 : 1+gen()%100;
      long long y1=1+gen()%1000000000, y2=y1+gen()%(1000000001-y1);
      if(trial%4==0) y2=y1; // 超出原题的零宽退化回归
      a.push_back({x,y1,y2});
    }
    int answer=0;
    for(int k=0;k<=n;++k) {
      bool expected=exact_prefix(a,k), actual=check_prefix(a,k);
      if(expected!=actual) {
        cerr<<"P3222 mismatch "<<trial<<" prefix "<<k<<'\n';
        for(auto t:a) cerr<<t[0]<<' '<<t[1]<<' '<<t[2]<<'\n';
        return 1;
      }
      if(expected) answer=k;
    }
    assert(solve(a)==answer);
  }
  // 满规模、互不相交的靶子，已知抛物线 a=-1e-9,b=2 全部命中。
  vector<Target> large;
  for(long long x=1;x<=100000;++x) {
    long long floor_y=2*x-(x*x+999999999)/1000000000;
    large.push_back({x,max(1LL,floor_y),floor_y+2});
  }
  shuffle(large.begin(),large.end(),gen);
  assert(solve(large)==100000);
  cerr<<"P3222 tests passed: 3000 exact prefix cases + n=100000\n";
}
