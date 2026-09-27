#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using LD = long double;
#include "../src/sections/LinearAlgebra/assets/Math/高斯消元.cpp"
#define main simplex_handbook_main
#include "../src/sections/LinearAlgebra/assets/Math/Simplex.cpp"
#undef main

void load_simplex(const vector<LD>& c, const vector<vector<LD>>& constraints) {
	Simplex::n = (int)c.size();
	Simplex::m = (int)constraints.size();
	for (auto& row : Simplex::a) fill(row, row + N, 0);
	for (int j = 1; j <= Simplex::n; j++) Simplex::a[0][j] = c[j - 1];
	for (int i = 1; i <= Simplex::m; i++) {
		assert((int)constraints[i - 1].size() == Simplex::n + 1);
		Simplex::a[i][0] = constraints[i - 1].back();
		for (int j = 1; j <= Simplex::n; j++)
			Simplex::a[i][j] = -constraints[i - 1][j - 1];
	}
}

void check_simplex(const vector<LD>& c, const vector<vector<LD>>& constraints,
		LD expected) {
	load_simplex(c, constraints);
	assert(Simplex::solve());
	assert(isfinite(Simplex::ans()));
	assert(abs(Simplex::ans() - expected) <= 1e-9L + 1e-12L * abs(expected));
	vector<LD> x(c.size());
	for (int i = 1; i <= Simplex::m; i++) {
		int variable = Simplex::id[Simplex::n + i];
		if (variable <= Simplex::n) x[variable - 1] = Simplex::a[i][0];
	}
	for (LD value : x) assert(value >= -1e-9L);
	for (const auto& row : constraints) {
		LD lhs = 0, scale = abs(row.back());
		for (int j = 0; j < (int)c.size(); j++) {
			lhs += row[j] * x[j];
			scale += abs(row[j] * x[j]);
		}
		assert(lhs - row.back() <= 1e-9L + 1e-12L * scale);
	}
	LD value = 0;
	for (int j = 0; j < (int)c.size(); j++) value += c[j] * x[j];
	assert(abs(value - expected) <= 1e-9L + 1e-12L * abs(expected));
}

int main() {
	{
		auto [rank, x] = gauss(vector<vector<LD>>{
			{1, 1, 3},
			{2, -1, 0},
		});
		assert(rank == 2);
		assert(abs(x[0] - 1) < 1e-12L);
		assert(abs(x[1] - 2) < 1e-12L);
	}
	{
		// 向上消元时，小系数乘以很大的解仍有不可忽略的贡献。
		auto [rank, x] = gauss(vector<vector<LD>>{
			{1, 1e-13L, 1},
			{0, 1e-11L, 100},
		});
		assert(rank == 2);
		assert(abs(x[0]) < 1e-12L);
		assert(abs(x[1] / 1e13L - 1) < 1e-12L);
		assert(abs(x[0] + 1e-13L * x[1] - 1) < 1e-12L);
		assert(abs(1e-11L * x[1] - 100) < 1e-10L);
	}
	{
		// 向下消元时，不能仅因系数不超过 eps 就跳过更新。
		auto [rank, x] = gauss(vector<vector<LD>>{
			{1, 1e12L, 0},
			{1e-12L, 2, 1},
		});
		assert(rank == 2);
		assert(abs(x[0] / 1e12L + 1) < 1e-12L);
		assert(abs(x[1] - 1) < 1e-12L);
		assert(abs(x[0] / 1e12L + x[1]) < 1e-12L);
		assert(abs(1e-12L * x[0] + 2 * x[1] - 1) < 1e-12L);
	}
	{
		auto [rank, x] = gauss(vector<vector<LD>>{
			{1, 1, 1, 2},
			{2, 2, 2, 4},
		});
		assert(rank == 1);
		assert(abs(x[0] + x[1] + x[2] - 2) < 1e-12L);
	}
	{
		auto [rank, x] = gauss(vector<vector<LD>>{
			{1, 1, 1},
			{2, 2, 3},
		});
		assert(rank == -1);
		assert(x.empty());
	}
	{
		auto [rank, x] = gauss_mod(vector<vector<int>>{
			{1, 1, 3},
			{2, -1, 0},
		}, 7);
		assert(rank == 2);
		assert((x == vector<int>{1, 2}));
	}
	{
		auto [rank, x] = gauss_mod(vector<vector<int>>{
			{1, 1, 1, 2},
			{2, 2, 2, 4},
		}, 7);
		assert(rank == 1);
		assert((x[0] + x[1] + x[2]) % 7 == 2);
	}
	{
		auto [rank, x] = gauss_mod(vector<vector<int>>{
			{1, 1, 1},
			{2, 2, 3},
		}, 7);
		assert(rank == -1);
		assert(x.empty());
	}
	static_assert(sizeof(Simplex::id) / sizeof(Simplex::id[0]) == 2 * N);
	// 有限最优值达到或超过旧版 INF，仍应正常选出出基行。
	for (LD bound : {1e9L, 1e9L + 1, 1e12L})
		check_simplex({1}, {{1, bound}}, bound);
	// 最大检验数规则会循环的退化问题，最优解 (1,0,1,0)，最优值 1。
	check_simplex({10, -57, -9, -24}, {
		{0.5L, -5.5L, -2.5L, 9, 0},
		{0.5L, -1.5L, -0.5L, 1, 0},
		{1, 0, 0, 0, 1},
	}, 1);
	// 负常数项需要先寻找可行基；连续调用也要重新初始化变量编号。
	check_simplex({3, 2}, {{-1, 0, -1}, {0, -1, -2}, {1, 1, 5}}, 13);
	check_simplex({-1}, {{-1, -2}}, -2);
	check_simplex({0}, {}, 0);
	load_simplex({1}, {{1, 0}, {-1, -1}});
	assert(!Simplex::solve()); // 不可行
	load_simplex({1}, {{-1, 0}});
	assert(!Simplex::solve()); // 无界
	// 松弛变量编号可达到 N，output 必须过滤它们，避免访问 tp[N]。
	check_simplex({1}, vector<vector<LD>>(N - 1, {1, 2}), 2);
	Simplex::output();
	assert(Simplex::tp[1] > 0);
	assert(abs(Simplex::a[Simplex::tp[1]][0] - 2) < 1e-12L);
	Simplex::n = 1; Simplex::m = 0; Simplex::tp[1] = 1;
	Simplex::output();
	assert(Simplex::tp[1] == 0);
}
