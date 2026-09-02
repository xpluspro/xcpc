// 子集和，先维度后 mask
for (int mask = 0; mask < (1 << N); ++mask) {
    F[mask] = A[mask]; }
for (int i = 0; i < N; ++i) { 
    for (int mask = 0; mask < (1 << N); ++mask) {
        if (mask & (1 << i)) { 
            F[mask] += F[mask ^ (1 << i)]; } } }
// 超集和
for (int i = 0; i < N; ++i) {
    for (int mask = (1 << N) - 1; mask >= 0; --mask) {
        if (!(mask & (1 << i))) {
            F[mask] += F[mask ^ (1 << i)]; } } }