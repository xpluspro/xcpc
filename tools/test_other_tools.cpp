#include <bits/stdc++.h>
using namespace std;

#include "../src/sections/OtherTools/assets/Miscellany/hack.cpp"

int main() {
	bitset<8> bits("10100100");
	vector<size_t> positions;
	ForEachSetBit(bits, [&](size_t index) { positions.push_back(index); });
	if (positions != vector<size_t>({2, 5, 7})) return 1;

	unsigned long long result = 0;
	if (AddOverflow(1, 2, result) || result != 3) return 1;
	if (!AddOverflow(ULLONG_MAX, 1, result) || result != 0) return 1;
	if (HashFunc<int>{}(42) != hash<int>{}(42)) return 1;
	if (SteadyClockSeed() <= 0) return 1;
	GospersHack(2, 5);
	return 0;
}
