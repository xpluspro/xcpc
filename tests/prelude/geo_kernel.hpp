#pragma once

// Handbook 2D kernel (8.1–8.3). Do not include common.hpp in the same TU:
// 8.1 #define-s LD/INF which collide with the contest prelude.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <math.h>
#include <random>
#include <vector>

// Handbook snippets call acosl/cosl/fabsl/sinl/sqrtl unqualified. Those C
// long-double names live in the global namespace (via <math.h> / <cmath>);
// libstdc++ does not put them in std, so `using std::acosl` fails on Ubuntu.
using std::abs;
using std::clamp;
using std::max;
using std::min;
using std::mt19937;
using std::sort;
using std::unique;
using std::vector;

#include "src/sections/ComputationalGeometry/assets/Geometry/8.1-declarations.cpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp"
#include "src/sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp"
