int lucas(int n, int m) {
  if(n % p < m % p) return 0;
  if(n < p) return binom(n, m);
  return 1ll * lucas(n / p, m / p) * binom(n % p, m % p) % p;
}
// n choose m 是奇数当且仅当 m 在二进制下每一位均不大于 n ，当且仅当 n 和 m 的按位与等于 m , 当且仅当 n 和 m 的按位或等于 n