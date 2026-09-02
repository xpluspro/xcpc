#define sz(cur) T[cur].sz
#define fa(cur) T[cur].fa
#define ls(cur) T[cur].ls
#define rs(cur) T[cur].rs
#define mid ((l+r)>>1)
#define lson ls(rt),l,mid
#define rson rs(rt),mid+1,r
struct Tree{
    int ls,rs,sz,fa;
}T[N*400];  // big enough

void Modify(int pre,int &rt,int l,int r,int x,int _sz,int _fa){
    rt=++tot;
    T[rt]=T[pre];
    if(l==r){
        assert(l==x); 
        sz(rt)=_sz;
        fa(rt)=_fa;
        return; 
    }
    if(x<=mid) Modify(ls(pre),lson,x,_sz,_fa);
    else Modify(rs(pre),rson,x,_sz,_fa);
}
pair<int,int> Ask(int rt,int l,int r,int x){
    if(!rt) assert(0);
    if(l==r){
        assert(l==x);
        return make_pair(sz(rt),fa(rt));
    }
    if(x<=mid) return Ask(lson,x);
    else return Ask(rson,x);
}
pair<int,int> Find(int now) {
    pair<int,int> tmp=Ask(cur,1,n,now);
    while(tmp.second!=now) {
        now=tmp.second;
        tmp=Ask(cur,1,n,now);
    }
    return tmp;
}
void Merge(int a,int b){
    pair<int,int>A=Find(a);
    pair<int,int>B=Find(b);
    if(A.second==B.second) return ;
    if(A.first>B.first) swap(A,B);
    Modify(cur,root[++opt],1,n,B.second,A.first+B.first,B.second);
    Modify(root[opt],root[1+opt],1,n,A.second,0,B.second); ++opt;
    cur=root[opt];
}
void build(int &rt,int l,int r){
    rt=++tot;
    if(l==r) {
        sz(rt)=1;
        fa(rt)=l;
        return ;
    }
    build(lson);
    build(rson);
}