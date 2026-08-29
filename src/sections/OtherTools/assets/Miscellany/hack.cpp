//fast = O3 + ffast-math + fallow-store-data-races
#pragma GCC optimize("Ofast")
//下：加include后面|   位运算   |浮点|  SIMD   | 优化寄存器 |
#pragma GCC target("abm,bmi,bmi2,fma,sse2,avx2,tune=native")
const int SZ = 1 << 16; int getc() {
	static char buf[SZ], *ptr = buf, *top = buf;
	if (ptr == top) {
		ptr = buf, top = buf + fread(buf, 1, SZ, stdin);
		if (top == buf) return -1; }
	return *ptr++; }
int read() {
	int x = 0, f = 1, ch = getc();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getc();
	}
	while (isdigit(ch)) x = x * 10 + ch - '0', ch = getc();
	return x * f;
}
const int OSZ = 1 << 16;
char obuf[OSZ]; int opos = 0;
void flush() {
	fwrite(obuf, 1, opos, stdout);
	opos = 0;
}
struct Flusher { ~Flusher() { flush(); } } flusher;
void pc(char c) {
	if (opos == OSZ) flush();
	obuf[opos++] = c;
}
void writeInt(long long x, char end = '\n') {
	unsigned long long y;
	if (x < 0) pc('-'), y = 0ULL - (unsigned long long)x;
	else y = x;
	char s[24]; int n = 0;
	do s[n++] = char('0' + y % 10); while (y /= 10);
	while (n) pc(s[--n]);
	pc(end);
}
void writeStr(const char *s, char end = '\n') {
	while (*s) pc(*s++);
	pc(end);
}
idx=b._Find_first();idx!=b.size();idx=b._Find_next(idx);
struct HashFunc{size_t operator()(const KEY &key)const{}};
__builtin_uaddll_overflow(a, b, &c) // binary big int
void GospersHack(int k, int n) {
	for (int s = (1 << k) - 1, c, r; s < (1 << n);
	c = s & -s, r = s + c, s = (((r ^ s) >> 2) / c) | r); }
chrono::steady_clock::now().time_since_epoch().count();
