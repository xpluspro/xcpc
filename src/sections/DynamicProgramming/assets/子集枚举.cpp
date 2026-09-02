for (int mask = 0; mask < (1 << N); ++mask) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) { }}
    // 如果包含空集 sub = 0，单独处理即可 