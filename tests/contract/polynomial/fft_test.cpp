#include <gtest/gtest.h>
#define N 1024
#define MAXN 1024
#include "prelude/common.hpp"
#include "src/sections/Polynomial/assets/Math/FFT.cpp"


TEST(Fft, RoundTrip) {
  fft_init(64);
  for (int n : {2, 4, 8, 16, 32}) {
    vector<cp> a(n);
    for (int i = 0; i < n; ++i) a[i] = cp((double)i, 0.0);
    fft(a.data(), n, 1);
    fft(a.data(), n, -1);
    for (int i = 0; i < n; ++i) EXPECT_NEAR(a[i].real(), (double)i, 1e-8);
  }
}

TEST(Fft, ConvolutionExactSmall) {
  fft_init(64);
  vector<int> A = {1, 2, 3, 4}, B = {5, 6, 7, 8};
  int n = 16;
  vector<cp> a(n), b(n);
  for (int i = 0; i < 4; ++i) a[i] = A[i], b[i] = B[i];
  fft(a.data(), n, 1);
  fft(b.data(), n, 1);
  for (int i = 0; i < n; ++i) a[i] *= b[i];
  fft(a.data(), n, -1);
  vector<int> want(7);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) want[i + j] += A[i] * B[j];
  for (int i = 0; i < 7; ++i) EXPECT_NEAR(a[i].real(), want[i], 1e-6);
}


