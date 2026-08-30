#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
using LD = long double;
#define main simplex_handbook_main
#include "../src/sections/LinearAlgebra/assets/Math/Simplex.cpp"
#undef main

int main() {
	static_assert(sizeof(Simplex::id) / sizeof(Simplex::id[0]) == 2 * N);
	Simplex::n = 1; Simplex::m = 0; Simplex::tp[1] = 1;
	Simplex::output();
	assert(Simplex::tp[1] == 0);
}
