#include<bits/stdc++.h>
using namespace std;
const int N=2e5+10; 
struct LCT{
    int val,Size,Xval,ch[2];
    bool lazy;
}S[N*2]; 
int f[N]; 
void Pushup(int cur) {
    S[cur].Xval=S[S[cur].ch[0]].Xval xor S[S[cur].ch[1]].Xval xor S[cur].val;
    S[cur].Size=S[S[cur].ch[0]].Size+S[S[cur].ch[1]].Size+1; 
}
void Reverse(int cur) {
    // cout<<"Reverse : "<<cur<<endl; 
    int tmp=S[cur].ch[0];
    S[cur].lazy^=1;     
    S[cur].ch[0]=S[cur].ch[1]; 
    S[cur].ch[1]=tmp; 
    //Pushup(cur) // 若左右节点的顺序会影响值的话，必须 Pushup 
}
void Pushdown(int cur){
    if(!S[cur].lazy) return ; 
    else{
        S[cur].lazy=false;  
        if(S[cur].ch[0]) Reverse(S[cur].ch[0]);
        if(S[cur].ch[1]) Reverse(S[cur].ch[1]); 
    }// Pushup(cur); 若左右节点的顺序会影响值的话，必须 Pushup 
}
bool check(int cur) {  
    return cur==S[f[cur]].ch[1];
}
bool isroot(int cur) {
    return cur!=S[f[cur]].ch[0]&&cur!=S[f[cur]].ch[1]; 
}
void Rotate(int cur) {  

    int F=f[cur],GF=f[F],op=(cur==S[F].ch[1]),Tmp=S[cur].ch[!op];
    if(!isroot(F)) S[GF].ch[F==S[GF].ch[1]]=cur;  // 如果是 Root ，就不能将 GrandFather 的 ch 重定向！！！
    S[F].ch[op]=Tmp;
    S[cur].ch[op^1]=F;
    if(Tmp) f[Tmp]=F; 
    f[F]=cur; f[cur]=GF; 
    Pushup(F);
    Pushup(cur); 
    // 如果这里子树的值与子树内排布有关，还需要 Pushup(GF);
}
int st[N];
void Splay(int cur) {
    // cout<<"Splay : "<<cur<<endl; 
    int tmp=cur,top=0;
    st[++top]=tmp; 
    while(!isroot(tmp)) st[++top]=tmp=f[tmp];
    for(int i=top;i>0;--i) {
        Pushdown(st[i]); 
    }
    // Splay 一定是双旋的！！！！ 单旋是会被卡成 O(N^2) 的！！！
    for(int F=f[cur];!isroot(cur);Rotate(cur),F=f[cur]){
        if(!isroot(F)) {
            Rotate(check(F)==check(cur)?F:cur); 
        }
    }
}
void Access(int cur) {
    // cout<<"Access  : "<<cur<<endl; 
    for(int lst=0;cur;lst=cur,cur=f[cur]) {
        // cout<<" giving_up "<<cur<<" -> "<<S[cur].ch[1]<<endl; 
        // cout<<" connect : "<<cur<<" -> "<<lst<<endl; 
        Splay(cur); S[cur].ch[1]=lst; Pushup(cur); // 这里需要注意，你得要保证空节点没有任何形式的贡献
    }
}
void makeroot(int cur) {
    Access(cur); //Access不会带上尾巴！ S[cur].ch[1]=lst 语句实现了去除比 cur 深度更大的链条！
    Splay(cur);
    Reverse(cur); 
}
int split(int x,int y) {
    makeroot(x); 
    Access(y); 
    Splay(y); 
    return S[y].Xval; 
}
int findroot(int cur) {
    Access(cur); Splay(cur);
    while(S[cur].ch[0]) Pushdown(cur),cur=S[cur].ch[0];
    Splay(cur); // 保证 Splay 的势能正确性
    return cur; 
}
void Add(int x,int y) {
    
    // cout<<"Add"
    makeroot(x); 
    if(findroot(y)==x) return ;
    f[x]=y;
    // cout<<"Add : "<<y<<" -> "<<x<<endl; 
    // Pushup(x);
}
void Del(int x,int y) {
    makeroot(x);
    if(findroot(y)==x&&S[x].Size==2)
    {
        f[y]=S[x].ch[1]=0;
        Pushup(x);
    }
}
int main()
{
    // freopen("LCT.in","r",stdin);
    // freopen("LCT.out","w",stdout); 
    int n,m;  
    cin>>n>>m;
    for(int i=1;i<=n;++i) {
        cin>>S[i].val; 
    }
    for(int i=1;i<=m;++i) {
        int op,x,y;
        cin>>op>>x>>y;
        if(op==0){
            cout<<split(x,y)<<endl; 
        }
        else if(op==1) {
            Add(x,y) ;
        }
        else if(op==2) {
            Del(x,y);
        }
        else {
            Splay(x); 
            S[x].val=y; 
            Pushup(x); 
        }
    }
    return 0; 
}