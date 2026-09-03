// FHQ treap

void push(int x){sz[x]=sz[ls[x]]+sz[rs[x]]+1;}
void spl(int p,int v,int &x,int &y){
	if(!p)return x=y=0,void();
	if(val[p]<=v)spl(rs[p],v,rs[x=p],y);
	else spl(ls[p],v,x,ls[y=p]);
	push(p);
}
void spl(int p,int v,int &x,int &y){
	if(!p)return x=y=0,void();
	if(v<=sz[ls[p]])spl(ls[p],v,x,ls[y=p]);
	else spl(rs[p],v-sz[ls[p]]-1,rs[x=p],y);
	push(x);
}
int mer(int x,int y){
	if(!x||!y)return x|y;
	if(rd[x]<rd[y])return ls[y]=mer(x,ls[y]),push(y),y;
	return rs[x]=mer(rs[x],y),push(x),x;
}
int nd(int v){
	int x=++node;
	val[x]=v,rd[x]=rand(),sz[x]=1;
	return x;
}
void ins(int v){
	int x=0,y=0;
	spl(R,v-1,x,y),R=mer(mer(x,nd(v)),y);
}
void del(int v){
	int x=0,y=0,z=0;
	spl(R,v,x,z),spl(x,v-1,x,y);
	R=mer(mer(x,y=mer(ls[y],rs[y])),z);
}
int kth(int k){
	int p=R;
	while(1){
		if(k<=sz[ls[p]])p=ls[p];
		else if(k==sz[ls[p]])return val[p];
		else k-=sz[ls[p]]+1,p=rs[p];
	}
}
int pre(int v){
	int p=R,ans=0;
	while(1){
		if(!p)return ans;
		else if(v<=val[p])p=ls[p];
		else ans=val[p],p=rs[p];
	}
}
int suc(int v){
	int p=R,ans=0;
	while(1){
		if(!p)return ans;
		else if(v>=val[p])p=rs[p];
		else ans=val[p],p=ls[p];
	}
}
int rnk(int v){
	int x=0,y=0,ans;
	spl(R,v-1,x,y),ans=sz[x]+1;
	return R=mer(x,y),ans;
}
