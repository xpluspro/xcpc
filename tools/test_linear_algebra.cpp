#include <algorithm>
#include <cassert>
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
	Simplex::n = 1; Simplex::m = 0; Simplex::tp[1] = 1;
	Simplex::output();
	assert(Simplex::tp[1] == 0);
}
