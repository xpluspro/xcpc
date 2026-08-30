#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using std::abs;
using std::array;
// libstdc++ bitset has _Find_first; libc++ does not. Provide a thin wrapper.
template <int LEN>
struct bitset {
  std::bitset<LEN> b;
  bitset() = default;
  void set() { b.set(); }
  void set(size_t i) { b.set(i); }
  void reset() { b.reset(); }
  void reset(size_t i) { b.reset(i); }
  bool test(size_t i) const { return b.test(i); }
  bitset &operator&=(const bitset &o) {
    b &= o.b;
    return *this;
  }
  bitset operator&(const bitset &o) const {
    bitset r = *this;
    r &= o;
    return r;
  }
  size_t _Find_first() const {
    for (int i = 0; i < LEN; ++i)
      if (b.test((size_t)i)) return (size_t)i;
    return (size_t)LEN;
  }
};
using std::clamp;
using std::cerr;
using std::cin;
using std::complex;
using std::cout;
using std::deque;
using std::endl;
using std::fill;
using std::find;
using std::function;
using std::gcd;
using std::get;
using std::greater;
using std::ifstream;
using std::istream;
using std::make_pair;
using std::make_tuple;
using std::map;
using std::max;
using std::memcpy;
using std::memset;
using std::min;
using std::multiset;
using std::mt19937_64;
using std::numeric_limits;
using std::ostream;
using std::pair;
using std::polar;
using std::priority_queue;
using std::queue;
using std::set;
using std::shuffle;
using std::sort;
using std::stack;
using std::string;
using std::swap;
using std::tuple;
using std::vector;

using LL = long long;
using ll = long long;
using ULL = unsigned long long;
using ull = unsigned long long;
using LD = long double;
using i128 = __int128;
using u128 = unsigned __int128;
inline LD max(LD a, LD b) { return a > b ? a : b; }
inline LD min(LD a, LD b) { return a < b ? a : b; }

// Do not #define N or M here: GoogleTest uses those identifiers as template
// parameters. Tests that need array caps #define them after including gtest.

constexpr LD PI_LD = 3.14159265358979323846264338327950288L;

#ifndef INF
#define INF 0x3f3f3f3f
#endif

constexpr LD EPS_LD = 1e-12L;

inline int sgn(LD x) { return x < -1e-12L ? -1 : (x > 1e-12L ? 1 : 0); }
inline int sgn(double x) { return x < -1e-12 ? -1 : (x > 1e-12 ? 1 : 0); }
inline int sgn(LL x) { return (x > 0) - (x < 0); }
inline int sgn(int x) { return (x > 0) - (x < 0); }

inline LL qpow(LL a, LL k, LL mod) {
  if (mod <= 0) return 0;
  LL r = 1 % mod;
  a %= mod;
  if (a < 0) a += mod;
  while (k > 0) {
    if (k & 1) r = (LL)((i128)r * a % mod);
    a = (LL)((i128)a * a % mod);
    k >>= 1;
  }
  return r;
}

template <class T>
T gcd_signed(T a, T b) {
  if (a < 0) a = -a;
  if (b < 0) b = -b;
  while (b) {
    T t = a % b;
    a = b;
    b = t;
  }
  return a;
}
#ifndef __gcd
#define __gcd gcd_signed
#endif

inline LL mul(LL a, LL b, LL m) {
  LL r = (LL)((i128)a * b % m);
  if (r < 0) r += m;
  return r;
}

#define Pw qpow
#define fr(i, a, b) for (int i = (a); i <= (b); ++i)

inline LL sqr(LL x) { return x * x; }
inline LD sqr(LD x) { return x * x; }
inline double sqr(double x) { return x * x; }
