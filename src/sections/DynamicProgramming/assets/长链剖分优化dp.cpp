// 使用场景：转移状态仅与“节点深度/距离”相关的树形 DP
int depth[N], son[N], tmp[N], * dp[N], * ptr = tmp;
void dfs1(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs1(v, u);
        if (depth[v] > depth[son[u]]) son[u] = v; }
    depth[u] = depth[son[u]] + 1; }

void dfs2(int u, int p) {
    dp[u][0] = 1; // 深度为 0 的节点（u 本身）
    if (son[u]) {
        dp[son[u]] = dp[u] + 1; // O(1) 继承重儿子内存空间
        dfs2(son[u], u); }
    for (int v : adj[u]) {
        if (v == p || v == son[u]) continue;
        dp[v] = ptr; ptr += depth[v]; // 给轻链分配新的内存块
        dfs2(v, u);
        // 轻儿子合并到父节点，循环次数为轻链长度
        for (int i = 0; i < depth[v]; ++i) {
            dp[u][i + 1] += dp[v][i]; } } }

// 在 main 中初始化根节点的指针：
// dp[1] = ptr; ptr += depth[1];