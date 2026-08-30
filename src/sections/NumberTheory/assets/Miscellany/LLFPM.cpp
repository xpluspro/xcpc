LL modmul(LL a, LL b, LL M) { // skip2004, M < 63bit
	LL ret = (LL)((__int128)a * b % M);
	return ret < 0 ? ret + M : ret; }
ULL modmul(ULL a, ULL b, LL M) { // orz@CF, M in 63 bit
	LL ret = (LL)((__int128)a * b % M);
	return ret < 0 ? ret + M : ret; }
// use int128 instead if M > 63 bit
struct DIV { 
	ULL p, ip;
	void init (ULL _p) { p = _p; ip = -1llu / p; }
	int mod (ULL x) { // x < 2 ^ 64
		ULL q = ULL(((u128)ip * x) >> 64);
		ULL r = x - q * p;
		return int(r >= p ? r - p : r);
} }; // speedup only when mod is not const