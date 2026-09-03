struct StringHash {
    int B = 131;
    int M = 1e9 + 7;
    int n = 0;
    vector<int> h, p;
    StringHash() {}
    StringHash(int base, int mod) : B(base), M(mod) {}
    void init(int base, int mod) {
        B = base;
        M = mod;
    }
    void build(const string& s) {
        n = s.size();
        h.assign(n + 1, 0);
        p.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) {
            p[i] = 1LL * p[i - 1] * B % M;
            h[i] = (1LL * h[i - 1] * B + s[i - 1]) % M;
        }
    }
    void build(const string& s, int base, int mod) {
        init(base, mod);
        build(s);
    }
    int get(int l, int r) const {
        int res = (h[r] - 1LL * h[l - 1] * p[r - l + 1]) % M;
        if (res < 0) res += M;
        return res;
    }
};