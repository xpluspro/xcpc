#include<bits/stdc++.h>
#define ls(op) (T[op].ls)
#define rs(op) (T[op].rs)
#define val(p) (T[p].val)
#define sz(p) (T[p].sz) 
#define prio(p) (T[p].prio)
using namespace std;
const int N=1e5+10; 
struct FHQ{int val,sz,ls,rs,prio;} T[N]; 
int cnt,root;
void pushup(int rt) {
    sz(rt)=sz(ls(rt))+sz(rs(rt))+1; 
}
mt19937 rng(random_device{}()); 
int New(int val){
    ++cnt; sz(cnt)=1; val(cnt)=val; prio(cnt)=rng(); return cnt; 
}   
void spilit(int rt,int val,int &a,int &b) {
    if(!rt) return a=b=0,void();
    if(val(rt)<=val) spilit(rs(rt),val,rs(a=rt),b);
    else spilit(ls(rt),val,a,ls(b=rt)); 
    pushup(rt); 
}
int merge(int a,int b) {
    if(!a||!b) return a|b;
    if(prio(a)>prio(b)) return rs(a)=merge(rs(a),b),pushup(a),a;
    else return ls(b)=merge(a,ls(b)),pushup(b),b;
}
int pre(int val) {
    int rt=root,ans=0; 
    while(rt) {
        if(val(rt)<val) ans=val(rt),rt=rs(rt);
        else rt=ls(rt);
    }
    return ans; 
} 
int nxt(int val) {
    int rt=root,ans=0;
    while(rt) {
        if(val(rt)>val) ans=val(rt),rt=ls(rt);
        else rt=rs(rt); 
    }
    return ans; 
}
int kth(int rk) {
    int rt=root; 
    while(rt) {
        if(sz(ls(rt))>=rk) rt=ls(rt);
        else if(sz(ls(rt))+1==rk) return val(rt);
        else rk-=sz(ls(rt))+1,rt=rs(rt);
    }
    assert(0); 
}
void del(int val){
    int a,b,c;
    spilit(root,val,b,c);
    spilit(b,val-1,a,b);
    root=merge(merge(a,merge(ls(b),rs(b))),c); 
}
void Insert(int val) {
    int a,b; 
    spilit(root,val-1,a,b);
    root=merge(merge(a,New(val)),b);
}
int rk(int val ){
    int a,b,ans; 
    spilit(root,val-1,a,b); ans=sz(a)+1; 
    root=merge(a,b); 
    return ans; 
}