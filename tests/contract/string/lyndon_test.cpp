#include <gtest/gtest.h>
#include "prelude/common.hpp"

int mn[1024], mx[1024];

#include "src/sections/String/assets/String/Lyndon Word.cpp"


TEST(Lyndon, MinSuffixEachPrefix) {
  vector<string> ts = {"a", "ba", "abcabc", "zzza", "mississippi"};
  for (auto t : ts) {
    int n = (int)t.size();
    char buf[1024];
    buf[0] = 0;
    for (int i = 0; i < n; ++i) buf[i + 1] = t[i];
    mnsuf(buf, mn, n);
    for (int p = 1; p <= n; ++p) {
      int best = 1;
      for (int i = 2; i <= p; ++i) {
        int k = 0;
        while (i + k <= p && buf[i + k] == buf[best + k]) ++k;
        if (i + k <= p && buf[i + k] < buf[best + k]) best = i;
        else if (i + k > p && best + k <= p) { /* prefix of suffix */ }
      }
      string got, want;
      for (int i = mn[p]; i <= p; ++i) got += buf[i];
      for (int i = best; i <= p; ++i) want += buf[i];
      // min suffix as string
      string mins = t.substr(p - 1, 1);
      for (int i = 0; i < p; ++i) {
        string suf = t.substr(i, p - i);
        if (suf < mins) mins = suf;
      }
      EXPECT_EQ(got, mins) << t << " prefix " << p;
    }
  }
}
