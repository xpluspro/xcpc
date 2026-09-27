void exgcd(int a, int b, int &x, int &y) {
  if(!b) return x = 1, y = 0, void();
  exgcd(b, a % b, y, x), y -= a / b * x;
}
int inv(int x, int p) {
  return exgcd(x, p, x, *new int), (x % p + p) % p;
}