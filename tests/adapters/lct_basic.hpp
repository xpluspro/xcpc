// SYNC_RANGE src/sections/DataStructures/assets/DataStructure/LCT.cpp 1-34
namespace LCT{
#define ls(x) c[x][0]
#define rs(x) c[x][1]
int val[N], ans[N], c[N][2], f[N], rev[N];
void push_up(int x){
	ans[x] = max({ans[ls(x)], ans[rs(x)], val[x]}); }
void opt(int x) {swap(ls(x), rs(x)), rev[x] ^= 1;}
void push_down(int x) {
	if(rev[x]) opt(ls(x)), opt(rs(x)), rev[x] = 0;}
bool nrt(int x) {return ls(f[x]) == x || rs(f[x]) == x;}
bool get(int x) {return x == rs(f[x]);}
void rot(int x){
	int y = f[x], z = f[y], t = get(x), w = c[x][t^1];
	if(nrt(y)) c[z][get(y)] = x;
	c[x][t^1] = y, c[y][t] = w;
	if(w) {f[w] = y;} f[y] = x, f[x] = z; push_up(y); }
void push_all(int x) {if(nrt(x)) push_all(f[x]); push_down(x);}
void splay(int x){
	push_all(x);
	while(nrt(x)){
		int y = f[x];
		if(nrt(y)) rot(get(x) == get(y) ? y : x);
		rot(x);
	} push_up(x); }
void access(int x) {for(int y=0;x;y=x, x=f[x]) splay(x), rs(x) = y, push_up(x);}
void mkrt(int x) {access(x); splay(x); opt(x);}
int findrt(int x){
	access(x), splay(x);
	while(ls(x)) push_down(x), x = ls(x);
	splay(x); return x; }
void split(int x, int y) {mkrt(x), access(y), splay(y);}
bool reach(int x, int y) {mkrt(x); return findrt(y) == x;}
void link(int x, int y) {if(!reach(x, y)) f[x] = y;}
void cut(int x, int y) {if(reach(x, y) && f[y] == x && !ls(y)) f[y] = rs(x) = 0;}
#undef ls
#undef rs
}
