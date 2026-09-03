// 输入序列 s，返回最短递推式的系数向量 C
// 使得 s[i] = sum_{j=0}^{L-1} C[j] * s[i - 1 - j]
vector<long long> berlekampMassey(const vector<long long>& s) {
    vector<long long> C, B; // C 为当前模型，B 为历史救星模型
    int L = 0, m = -1;
    long long dm = 0;
    for (int k = 0; k < (int)s.size(); ++k) {
        long long dk = s[k];
        for (int i = 0; i < L; ++i) {
            dk = (dk - C[i] * s[k - 1 - i] % MOD + MOD) % MOD; }
        if (dk == 0) continue; 
        vector<long long> oldC = C;
        long long shift = k - m;
        if (C.size() < B.size() + shift) C.resize(B.size() + shift, 0);
        long long scale = dk * modInverse(dm) % MOD;
        C[shift - 1] = (C[shift - 1] + scale) % MOD;
        for (int i = 0; i < (int)B.size(); ++i) {
            C[shift + i] = (C[shift + i] - scale * B[i] % MOD + MOD) % MOD; }
        if (k + 1 - L > L) {
            L = k + 1 - L; B = oldC; m = k; dm = dk; } }
    return C; }