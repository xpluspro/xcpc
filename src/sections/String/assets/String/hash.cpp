using u64 = uint64_t;
using u128 = __uint128_t;

struct StringHash {
    // 可换成上方表格中的任意 64 位大质数。
    static constexpr u64 DEFAULT_MOD = 1000000000000000003ULL;
    static constexpr u64 SECOND_MOD  = 1000000000000000009ULL;

    u64 B = 0, M = DEFAULT_MOD;
    int n = 0;
    vector<u64> h, p;

    static u64 default_base() {
        static const u64 value = [] {
            using Clock = chrono::steady_clock;
            mt19937_64 gen(Clock::now().time_since_epoch().count());
            return uniform_int_distribution<u64>(257, DEFAULT_MOD - 2)(gen);
        }();
        return value;
    }
    static u64 mul(u64 a, u64 b, u64 mod) {
        return u64(u128(a) * b % mod);
    }

    // 同一进程中的默认实例共享参数，因此可以直接比较。
    StringHash() : B(default_base()) {}
    StringHash(u64 base, u64 mod) { init(base, mod); }

    void init(u64 base, u64 mod) {
        assert(mod > 259 && 257 <= base && base < mod - 1);
        B = base;
        M = mod;
        n = 0;
        h.clear();
        p.clear();
    }
    void build(const string& s) {
        assert(s.size() <= size_t(INT_MAX));
        n = int(s.size());
        h.assign(n + 1, 0);
        p.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) {
            p[i] = mul(p[i - 1], B, M);
            u64 c = u64(static_cast<unsigned char>(s[i - 1])) + 1;
            h[i] = (mul(h[i - 1], B, M) + c) % M;
        }
    }
    void build(const string& s, u64 base, u64 mod) {
        init(base, mod);
        build(s);
    }

    // 返回 s[l..r] 的 Hash；l,r 是 1-based 闭区间。
    u64 get(int l, int r) const {
        assert(1 <= l && l <= r && r <= n);
        u64 removed = mul(h[l - 1], p[r - l + 1], M);
        return h[r] >= removed ? h[r] - removed : M - (removed - h[r]);
    }
};
