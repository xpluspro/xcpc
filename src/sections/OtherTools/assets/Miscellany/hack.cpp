#include <bits/stdc++.h>
using namespace std;

//fast = O3 + ffast-math + fallow-store-data-races
#pragma GCC optimize("Ofast")
//下：加include后面|   位运算   |浮点|  SIMD   | 优化寄存器 |
#pragma GCC target("abm,bmi,bmi2,fma,sse2,avx2,tune=native")
const int SZ = 1 << 16; int FastGetChar() {
	static char buf[SZ], *ptr = buf, *top = buf;
	if (ptr == top) {
		ptr = buf, top = buf + fread(buf, 1, SZ, stdin);
		if (top == buf) return -1; }
	return static_cast<unsigned char>(*ptr++); }
bool readInt(long long &x) {
	unsigned long long value = 0;
	int ch = FastGetChar();
	while (ch != -1 && ch != '-' && !isdigit(static_cast<unsigned char>(ch)))
		ch = FastGetChar();
	if (ch == -1) return false;
	bool negative = ch == '-';
	if (negative) ch = FastGetChar();
	if (ch == -1 || !isdigit(static_cast<unsigned char>(ch))) return false;
	const unsigned long long negativeLimit = 1ULL << 63;
	const unsigned long long limit = negative ? negativeLimit : LLONG_MAX;
	bool overflow = false;
	while (ch != -1 && isdigit(static_cast<unsigned char>(ch))) {
		unsigned digit = static_cast<unsigned>(ch - '0');
		if (value > (limit - digit) / 10) overflow = true;
		else if (!overflow) value = value * 10 + digit;
		ch = FastGetChar();
	}
	if (overflow) return false;
	if (negative && value == negativeLimit) x = LLONG_MIN;
	else x = negative ? -static_cast<long long>(value) : static_cast<long long>(value);
	return true;
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
template<class Bitset, class Func>
void ForEachSetBit(const Bitset &b, Func func) {
	for (size_t idx = b._Find_first(); idx != b.size(); idx = b._Find_next(idx))
		func(idx);
}
template<class Key> struct HashFunc {
	size_t operator()(const Key &key) const { return hash<Key>{}(key); }
};
bool AddOverflow(unsigned long long a, unsigned long long b,
		unsigned long long &c) { // binary big int
	return __builtin_uaddll_overflow(a, b, &c);
}
template<class Func>
void GospersHack(int k, int n, Func func) {
	assert(0 <= k && k <= n && n < 63);
	if (k == 0) { func(0ULL); return; }
	const unsigned long long limit = 1ULL << n;
	for (unsigned long long s = (1ULL << k) - 1; s < limit; ) {
		func(s);
		unsigned long long c = s & -s, r = s + c;
		s = (((r ^ s) >> 2) / c) | r;
	}
}
auto SteadyClockSeed() {
	return chrono::steady_clock::now().time_since_epoch().count();
}
