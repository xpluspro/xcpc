// ax^3 + bx^2 + cx + d = 0 的三个复根（计重数），要求 a != 0
using CubicComplex = complex<long double>;
array<CubicComplex, 3> solveCubic(long double a, long double b,
		long double c, long double d) {
	assert(a != 0);
	b /= a; c /= a; d /= a;
	long double p = c - b * b / 3;
	long double q = 2 * b * b * b / 27 - b * c / 3 + d;
	CubicComplex delta = q * q / 4 + p * p * p / 27;
	CubicComplex s = sqrt(delta), z1 = -q / 2 + s, z2 = -q / 2 - s;
	CubicComplex u = pow(abs(z1) >= abs(z2) ? z1 : z2, 1.L / 3);
	CubicComplex v = abs(u) == 0 ? 0 : -p / (3.L * u);
	CubicComplex w(-0.5L, sqrtl(3) / 2), shift = -b / 3;
	return {shift + u + v, shift + u * w + v * conj(w),
			shift + u * conj(w) + v * w};
}
