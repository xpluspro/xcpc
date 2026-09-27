// 依赖 3.14 StringHash（双 Hash）；返回 (l,r,p)，区间为 1-based 闭区间。
struct Runs { int l, r, p; };
bool operator==(const Runs& a, const Runs& b)
{ return a.l == b.l && a.r == b.r && a.p == b.p; }
using RunHash = array<StringHash, 2>;
int run_lcp(const RunHash& h, int n, int x, int y) { // x,y 为 0-based
    int l = 0, r = n - max(x, y);
    while (l < r) {
        int m = (l + r + 1) / 2;
        if (h[0].get(x + 1, x + m) == h[0].get(y + 1, y + m)
                && h[1].get(x + 1, x + m) == h[1].get(y + 1, y + m)) l = m;
        else r = m - 1;
    }
    return l;
}
vector<Runs> get_runs(const string& s) { // O(n log n)
    int n = s.size(); if (!n) return {};
    string t(s.rbegin(), s.rend());
    StringHash primary;
    RunHash f{primary, StringHash(primary.B, StringHash::SECOND_MOD)}, g = f;
    for (int i = 0; i < 2; ++i) f[i].build(s), g[i].build(t);
    vector<Runs> ans; vector<int> ly(n);
    auto less = [&](int x, int y) {
        int z = run_lcp(f, n, x, y);
        if (x + z == n || y + z == n) return x + z == n;
        return s[x + z] < s[y + z];
    };
    for (int op : {0, 1}) {
        vector<pair<int, int>> stk{{n - 1, n - 1}}; ly[n - 1] = n - 1;
        for (int i = n - 2; i >= 0; --i) {
            int r = i;
            while (!stk.empty() && less(i, stk.back().first) != op)
                r = stk.back().second, stk.pop_back();
            ly[i] = r; stk.push_back({i, r});
        }
        for (int l = 0; l < n; ++l) {
            int r = ly[l], p = r - l + 1, L = l, R = r;
            if (l) L -= run_lcp(g, n, n - l, n - 1 - r);
            if (r + 1 < n) R += run_lcp(f, n, l, r + 1);
            if (R - L + 1 >= 2 * p) ans.push_back({L + 1, R + 1, p});
        }
    }
    sort(ans.begin(), ans.end(), [](const Runs& a, const Runs& b) {
        return tie(a.l, a.r, a.p) < tie(b.l, b.r, b.p);
    });
    ans.erase(unique(ans.begin(), ans.end(), [](const Runs& a, const Runs& b) {
        return a.l == b.l && a.r == b.r;
    }), ans.end());
    return ans;
}
