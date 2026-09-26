#include<bits/stdc++.h>
#define ls(op) (T[op].ls)
#define rs(op) (T[op].rs)
#define val(p) (T[p].val)
#define sz(p) (T[p].sz) 
#define prio(p) (T[p].prio)
#define tag(p) (T[p].tag)
using namespace std;
const int N=1e5+10; 
struct FHQ{int val,sz,ls,rs,prio,tag;} T[N]; 
int cnt,root;
void pushup(int rt) {
    sz(rt)=sz(ls(rt))+sz(rs(rt))+1; 
}
void pushdown(int rt) {
    if(tag(rt)) {
        tag(ls(rt))^=tag(rt);
        tag(rs(rt))^=tag(rt); 
        swap(ls(rt),rs(rt));
        tag(rt)=0; 
    }
}
mt19937 rng(random_device{}()); 
int New(int val){
    ++cnt; sz(cnt)=1; val(cnt)=val; prio(cnt)=rng(); return cnt; 
}   
void spilit_ix(int rt,int val,int &a,int &b) {
    if(!rt) return a=b=0,void();
    pushdown(rt); 
    if(sz(ls(rt))>=val) spilit_ix(ls(rt),val,a,ls(b=rt)); 
    else spilit_ix(rs(rt),val-sz(ls(rt))-1,rs(a=rt),b); 
    pushup(rt); 
}
int merge(int a,int b) {
    if(!a||!b) return a|b;
    pushdown(a); pushdown(b);
    if(prio(a)>prio(b)) return rs(a)=merge(rs(a),b),pushup(a),a;
    else return ls(b)=merge(a,ls(b)),pushup(b),b;
}
void Modify(int l,int r) {
    int a,b,c; 
    spilit_ix(root,r,b,c); 
    spilit_ix(b,l-1,a,b);
    tag(b)^=1;
    root=merge(merge(a,b),c); 
} 
void print(int rt) {
    if(!rt) return ;
    pushdown(rt); 
    print(ls(rt)); 
    cout<<val(rt)<<" ";
    print(rs(rt));
}