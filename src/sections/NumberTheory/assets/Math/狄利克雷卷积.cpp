// Dirichlet 卷积：(f * g)(n) = sum_{d | n} f(d) g(n / d)。
// 复杂度 O(n log n)，结果类型和取模方式按题目自行替换。
template <class T>
vector<T> dirichlet_convolution(const vector<T>& f, const vector<T>& g) {
    int n = min(f.size(), g.size()) - 1; // 下标从 1 开始，f[0]、g[0] 不使用
    vector<T> h(n + 1);
    for (int d = 1; d <= n; ++d)
        for (int k = 1; d * k <= n; ++k)
            h[d * k] += f[d] * g[k];
    return h;
}
