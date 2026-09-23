#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

using namespace std;
using LD = long double;
#include "../src/sections/LinearAlgebra/assets/Geometry/minnorm_gauss.cpp"

LD norm_of(const Vec& v) {
	LD result = 0;
	for (LD value : v) result = hypot(result, value);
	return result;
}

MinNormResult check(const vector<Vec>& a, int d, int rank, const Vec& expected) {
	auto saved = a;
	auto result = minnorm_qr(a, d);
	assert(a == saved);
	assert(result.rank == rank);
	assert((int)result.x.size() == d);
	assert((int)result.kernel.size() == d - rank);
	for (int j = 0; j < d; j++) {
		assert(isfinite(result.x[j]));
		assert(abs(result.x[j] - expected[j]) < 1e-10L * (1 + abs(expected[j])));
	}
	// 可行性，以及每个基向量确实属于原矩阵的零空间。
	for (const auto& row : a) {
		LD largest = 0;
		for (int j = 0; j < d; j++) largest = max(largest, abs(row[j]));
		if (largest == 0) largest = 1;
		LD norm = 0;
		for (int j = 0; j < d; j++) norm = hypot(norm, row[j] / largest);
		if (norm == 0) norm = 1;
		LD residual = row[d] / largest / norm, magnitude = 1 + abs(residual);
		for (int j = 0; j < d; j++) {
			LD term = (row[j] / largest / norm) * result.x[j];
			residual += term;
			magnitude += abs(term);
		}
		assert(abs(residual) < 1e-9L * magnitude);
		for (const auto& v : result.kernel) {
			LD dot = 0;
			for (int j = 0; j < d; j++) dot += (row[j] / largest / norm) * v[j];
			assert(abs(dot) < 1e-9L);
		}
	}
	// 正交单位基 + 解与零空间正交，验证最小范数性质。
	for (int i = 0; i < (int)result.kernel.size(); i++) {
		assert((int)result.kernel[i].size() == d);
		LD dot = 0;
		for (int k = 0; k < d; k++) dot += result.x[k] * result.kernel[i][k];
		assert(abs(dot) < 1e-9L * (1 + norm_of(result.x)));
		for (int j = 0; j < (int)result.kernel.size(); j++) {
			dot = 0;
			for (int k = 0; k < d; k++) dot += result.kernel[i][k] * result.kernel[j][k];
			assert(abs(dot - (i == j ? 1 : 0)) < 1e-9L);
		}
	}
	return result;
}

void check_inconsistent(const vector<Vec>& a, int d) {
	auto result = minnorm_qr(a, d);
	assert(result.rank == -1);
	assert(result.x.empty() && result.kernel.empty());
}

int main() {
	check({{1, 1, -2}}, 2, 1, {1, 1});
	check({{1, 0, -2}, {0, 1, 3}, {1, 1, 1}}, 2, 2, {2, -3});
	check({{1, 1, 0, 0, -1}, {0, 0, 1, 0, -2}}, 4, 2, {0.5L, 0.5L, 2, 0});
	// 第二条约束相关，QR 必须交换后续列并同步处理右端。
	check({{1, 1, 0, -2}, {2, 2, 0, -4}, {0, 0, 1, -3}}, 3, 2, {1, 1, 3});
	check_inconsistent({{1, -1}, {1, -2}}, 1);
	check_inconsistent({{0, 0, 1}}, 2);
	// 旧版按 eps 跳过系数，错误地返回 x=1。
	check({{1, 1e-13L, -1}, {0, 1e-11L, -100}}, 2, 2, {0, 1e13L});
	// 旧版构造的 Gram 矩阵丢失对角线上的 +1，触发断言。
	for (LD large : {1e10L, 1e20L}) {
		LD denominator = 1 + 2 * large * large;
		Vec expected = {1 / denominator, large / denominator, large / denominator};
		auto result = check({{1, large, large, -1}}, 3, 1, expected);
		Vec error(3);
		for (int j = 0; j < 3; j++) error[j] = result.x[j] - expected[j];
		assert(norm_of(error) < 1e-9L * norm_of(expected));
	}
	// 等价方程缩放很多个数量级，仍应给出相同的秩和最小范数解。
	check({{1e-20L, 1e-20L, -2e-20L}, {1e20L, 1e20L, -2e20L}, {-3, -3, 6}},
		2, 1, {1, 1});
	// 每个输入都有限，但 hypot(max, max) 会溢出；解仍应是 (1/2, 1/2)。
	LD largest = numeric_limits<LD>::max();
	check({{largest, largest, -largest}}, 2, 1, {0.5L, 0.5L});
	check({}, 3, 0, {0, 0, 0});
	check({{0, 0, 0}}, 2, 0, {0, 0});
	check({}, 0, 0, {});
	check({{0}, {0}}, 0, 0, {});
	check_inconsistent({{1}}, 0);
	cout << "Minimum-norm QR regression tests passed\n";
}
