// 限制上下界，每次累加。倒序枚举父节点体积
void dfs(int u) {
    dp[u][v_cost[u]] = w_val[u];
    sz[u] = v_cost[u];
    for (int v : adj[u]) {
        dfs(v); 
        for (int j = min(V, sz[u] + sz[v]); j >= v_cost[u]; --j) {
            for (int k = 0; k <= min(sz[v], j - v_cost[u]); ++k) {
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[v][k]); } }
        sz[u] += sz[v]; } }