// 笛卡尔树

const int N = 1e7 + 5;
int stc[N], top, a[N], ls[N], rs[N];
void build() {
    for(int i = 1; i <= n; i++) {
        while(top && a[stc[top]] < a[i]) ls[i] = stc[top--];
        rs[stc[top]] = i, stc[++top] = i; // 注意这里可能会让 rs[0] = i, 但下标从 1 开始问题不大
    }
}