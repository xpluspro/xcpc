int merge(int l, int r, int x, int y) {
  if(!x || !y) return x | y;
  int m = l + r >> 1, z = ++node;
  if(l == r) return /* 合并叶子 x 和 y */, z;
  ls[z] = merge(l, m, ls[x], ls[y]);
  rs[z] = merge(m + 1, r, rs[x], rs[y]);
  return /* 合并左右儿子 */, z;
}