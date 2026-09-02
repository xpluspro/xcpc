int tot,fa[2*N];
int Find(int cur){
    return fa[cur]==cur?cur:fa[cur]=Find(fa[cur]);
}
void merge(int u,int v,int c) { 
    if((u=Find(u))==(v=Find(v))) return ;
    ++tot;
    fa[u]=fa[v]=tot;
    w[tot]=c;
}