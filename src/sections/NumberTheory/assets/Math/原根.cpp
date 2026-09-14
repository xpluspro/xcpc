// p 必须是质数；返回模 p 的最小原根。
int primitive_root_pow(int base, int exponent, int mod) {
	long long result = 1;
	for (; exponent; exponent >>= 1, base = (long long)base * base % mod)
		if (exponent & 1) result = result * base % mod;
	return (int)result;
}

int primitive_root_prime(int p) {
	if (p == 2) return 1;
	int phi = p - 1, remaining = phi;
	std::vector<int> factors;
	for (int divisor = 2; 1LL * divisor * divisor <= remaining; ++divisor)
		if (remaining % divisor == 0) {
			factors.push_back(divisor);
			while (remaining % divisor == 0) remaining /= divisor;
		}
	if (remaining > 1) factors.push_back(remaining);
	for (int candidate = 2; candidate < p; ++candidate) {
		bool valid = true;
		for (int factor : factors)
			if (primitive_root_pow(candidate, phi / factor, p) == 1) {
				valid = false;
				break;
			}
		if (valid) return candidate;
	}
	return -1; // p 为质数时不会到达
}
