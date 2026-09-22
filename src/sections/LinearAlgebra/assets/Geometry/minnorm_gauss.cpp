using Vec = vector<LD>;

struct MinNormResult {
    int rank; // -1：给定容差下一致性检查失败；否则为数值秩
    Vec x;
    vector<Vec> kernel; // 标准正交零空间基，每个 Vec 是一个基向量
};

// a 有 n 行、d+1 列，表示 sum_j a[i][j]*x[j] + a[i][d] = 0
// LD 用 long double；要求 d>=0、tol>0，输入均有限且各行等长
// 同时缩放各方程的系数和常数项，再判断数值秩；不修改输入 a
// tol 控制数值秩和残差容差，近相关约束仍需关注精度
// 无解或精度不足返回 {-1, {}, {}}，不求最小二乘拟合
MinNormResult minnorm_qr(const vector<Vec>& a, int d, LD tol = 1e-12L) {
    int n = (int)a.size();
    vector<Vec> r(d, Vec(n)), q(d, Vec(d));
    Vec scale(n, 1), rhs(n);
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    for (int i = 0; i < d; ++i) q[i][i] = 1;
    for (int i = 0; i < n; ++i) {
        LD norm = 0;
        for (int j = 0; j < d; ++j) norm = hypot(norm, a[i][j]);
        if (norm != 0) scale[i] = norm;
        rhs[i] = -a[i][d] / scale[i];
        for (int j = 0; j < d; ++j) r[j][i] = a[i][j] / scale[i];
    }

    // 缩放后的 A^T 做带列主元的 Householder QR：A^T P = Q R
    int rank = 0;
    for (int k = 0; k < min(n, d); ++k) {
        int best = k;
        LD best_norm = 0;
        for (int j = k; j < n; ++j) {
            LD norm = 0;
            for (int i = k; i < d; ++i) norm = hypot(norm, r[i][j]);
            if (norm > best_norm) best_norm = norm, best = j;
        }
        if (best_norm <= tol) break;
        swap(perm[k], perm[best]);
        for (int i = 0; i < d; ++i) swap(r[i][k], r[i][best]);

        // 取反号避免相近数相减；H = I - 2vv^T，v 为单位向量
        LD alpha = -copysign(best_norm, r[k][k]);
        Vec v(d - k);
        for (int i = k; i < d; ++i) v[i - k] = r[i][k];
        v[0] -= alpha;
        LD vnorm = 0;
        for (LD value : v) vnorm = hypot(vnorm, value);
        for (LD& value : v) value /= vnorm;

        // R <- H R；小内积也不能直接按 tol 跳过
        for (int j = k; j < n; ++j) {
            LD dot = 0;
            for (int i = k; i < d; ++i) dot += v[i - k] * r[i][j];
            for (int i = k; i < d; ++i) r[i][j] -= 2 * v[i - k] * dot;
        }
        r[k][k] = alpha;
        for (int i = k + 1; i < d; ++i) r[i][k] = 0;

        // Q <- Q H；保留完整 Q，以便返回零空间基
        for (int i = 0; i < d; ++i) {
            LD dot = 0;
            for (int j = k; j < d; ++j) dot += q[i][j] * v[j - k];
            for (int j = k; j < d; ++j) q[i][j] -= 2 * dot * v[j - k];
        }
        ++rank;
    }

    // 解 R11^T y = (P^T b) 的前 rank 项；其余正交坐标取 0
    Vec y(rank), x(d);
    for (int i = 0; i < rank; ++i) {
        LD value = rhs[perm[i]];
        for (int j = 0; j < i; ++j) value -= r[j][i] * y[j];
        y[i] = value / r[i][i];
    }
    for (int i = 0; i < d; ++i)
        for (int j = 0; j < rank; ++j) x[i] += q[i][j] * y[j];

    // 检查全部原始约束，包括相关行和零行
    for (int i = 0; i < n; ++i) {
        LD residual = -rhs[i], magnitude = 1 + abs(rhs[i]);
        for (int j = 0; j < d; ++j) {
            LD term = (a[i][j] / scale[i]) * x[j];
            residual += term;
            magnitude += abs(term);
        }
        if (!isfinite(residual) || abs(residual) > tol * magnitude)
            return {-1, {}, {}};
    }
    vector<Vec> kernel;
    for (int j = rank; j < d; ++j) {
        Vec v(d);
        for (int i = 0; i < d; ++i) v[i] = q[i][j];
        kernel.push_back(v);
    }
    return {rank, x, kernel};
}
