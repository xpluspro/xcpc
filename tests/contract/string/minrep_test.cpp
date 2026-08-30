#include <gtest/gtest.h>
#include "prelude/common.hpp"

int n;
int a[1024];

#include "src/sections/String/assets/String/最小表示法.cpp"


static int brute_minrep(const vector<int> &s) {
  int m = (int)s.size(), best = 0;
  for (int i = 1; i < m; ++i) {
    for (int k = 0; k < m; ++k) {
      int x = s[(i + k) % m], y = s[(best + k) % m];
      if (x < y) {
        best = i;
        break;
      }
      if (x > y) break;
    }
  }
  return best;
}

TEST(MinRep, EdgesAndRandom) {
  auto run = [](vector<int> s) {
    n = (int)s.size();
    for (int i = 0; i < n; ++i) a[i] = s[i];
    int got = minimal_string();
    int want = brute_minrep(s);
    vector<int> g(n), w(n);
    for (int i = 0; i < n; ++i) {
      g[i] = s[(got + i) % n];
      w[i] = s[(want + i) % n];
    }
    EXPECT_EQ(g, w);
  };
  run({1});
  run({1, 1, 1, 1});
  run({1, 2, 3, 1, 2, 3});
  run({3, 2, 1});
  run({2, 1, 2, 1});
  std::mt19937 rng(8);
  for (int t = 0; t < 100; ++t) {
    int m = rng() % 20 + 1;
    vector<int> s(m);
    for (int i = 0; i < m; ++i) s[i] = rng() % 4;
    run(s);
  }
}
