#include <algorithm>
#include <cassert>
#include <random>
#include <tuple>
#include <vector>

using namespace std;
using LL = long long;
const int p = 998244353, MOD = p, LOG = 63;
int qpow(int a, long long b) {
	int result = 1;
	for (; b; b >>= 1, a = (LL)a * a % p)
		if (b & 1) result = (LL)result * a % p;
	return result;
}
#include "../src/sections/Polynomial/assets/Math/Berlekamp-Massey.cpp"
#include "../src/sections/Polynomial/assets/Math/解稀疏方程组.cpp"
#include "../src/sections/Polynomial/assets/Math/线性递推.cpp"

using SparseMatrix = vector<tuple<int, int, int>>;
vector<int> matrix_vector(const SparseMatrix& matrix, const vector<int>& x) {
	vector<int> result(x.size());
	for (auto [i, j, value] : matrix)
		result[i] = (result[i] + (LL)value * x[j]) % p;
	return result;
}
void check_system(const SparseMatrix& matrix, const vector<int>& expected, unsigned seed) {
	const auto b = matrix_vector(matrix, expected);
	const auto answer = solve_sparse_equations(matrix, b, seed);
	assert(answer == expected);
	assert(matrix_vector(matrix, answer) == b);
}

int main() {
	assert((berlekamp_massey({}) == vector<int>{1}));
	assert((berlekamp_massey({0,0,0,0}) == vector<int>{1}));
	assert((berlekamp_massey({0,1,1,2,3,5,8,13}) == vector<int>{1,p-1,p-1}));

	// 只用前 2k 项恢复系数，检查未参与 BM 的后续项及 5.5 的系数转换。
	mt19937 cases(20260915);
	for (int order = 1; order <= 12; ++order) {
		for (int rep = 0; rep < 8; ++rep) {
			vector<int> trans(order), sequence(order);
			for (auto& value : trans) value = cases() % p;
			for (auto& value : sequence) value = cases() % p;
			if (rep == 0) fill(trans.begin(), trans.end(), 0);
			if (rep == 1) fill(sequence.begin(), sequence.end(), 0);
			for (int i = order; i < 6 * order + 8; ++i) {
				int value = 0;
				for (int j = 0; j < order; ++j)
					value = (value + (LL)trans[j] * sequence[i - j - 1]) % p;
				sequence.push_back(value);
			}
			auto c = berlekamp_massey(vector<int>(sequence.begin(), sequence.begin() + 2 * order));
			int degree = (int)c.size() - 1;
			assert(degree <= order && c[0] == 1);
			if (degree == 0) {
				assert(all_of(sequence.begin(), sequence.end(), [](int x) { return x == 0; }));
				continue;
			}
			vector<int> first(sequence.begin(), sequence.begin() + degree), recovered;
			for (int j = 1; j <= degree; ++j) recovered.push_back((p - c[j]) % p);
			LinearRec recurrence(first, recovered);
			for (int i = 0; i < (int)sequence.size(); ++i)
				assert(recurrence.calc(i) == sequence[i]);
		}
	}

	check_system({}, {}, 5489u);
	check_system({{0,0,1}}, {0}, 5489u);
	check_system({{0,0,7}}, {13}, 5489u);
	check_system({{0,0,5},{0,0,p-4}}, {19}, 5489u); // 重复条目按加法合并
	check_system({{0,0,1},{0,1,1},{1,0,1}}, {3,8}, 5489u);
	check_system({{0,1,1},{1,2,1},{2,0,1}}, {7,11,13}, 5489u);

	const SparseMatrix identity{{0,0,1},{1,1,1}};
	const vector<int> old_counterexample{135239155,184951029};
	assert(solve_sparse_equations(identity, old_counterexample) == old_counterexample);
	check_system(identity, old_counterexample, 5489u);

	// 精确制造第一轮投影丢失信息，必须重新投影才能返回正确答案。
	mt19937 projection(5489u);
	uniform_int_distribution<int> distribution(0, p - 1);
	vector<int> w{distribution(projection), distribution(projection), distribution(projection)};
	assert(w[0] != 0 && w[1] != 0 && w[2] != 0);
	vector<int> hidden{w[1], p - w[0]};
	assert(((LL)w[0] * hidden[0] + (LL)w[1] * hidden[1]) % p == 0);
	check_system(identity, hidden, 5489u); // 第一轮得到全零标量序列
	hidden.push_back(1);
	check_system({{0,0,1},{1,1,1},{2,2,2}}, hidden, 5489u); // 只看见特征值 2

	// 非零对角阵经可逆行操作生成矩阵；给定解后生成右端项，保证满秩。
	for (int n = 1; n <= 12; ++n) {
		for (int rep = 0; rep < 20; ++rep) {
			vector<vector<int>> dense(n, vector<int>(n));
			for (int i = 0; i < n; ++i) dense[i][i] = 1 + cases() % (p - 1);
			if (n > 1) for (int step = 0; step < 5 * n; ++step) {
				int i = cases() % n, j = cases() % (n - 1);
				if (j >= i) ++j;
				int factor = cases() % p;
				for (int k = 0; k < n; ++k)
					dense[i][k] = (dense[i][k] + (LL)factor * dense[j][k]) % p;
			}
			SparseMatrix matrix;
			for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
				if (dense[i][j]) matrix.emplace_back(i, j, dense[i][j]);
			vector<int> expected(n);
			for (auto& value : expected) value = cases() % p;
			if (rep == 0) fill(expected.begin(), expected.end(), 0);
			check_system(matrix, expected, cases());
		}
	}
}
