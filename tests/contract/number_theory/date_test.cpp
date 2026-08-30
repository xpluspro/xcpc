#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/NumberTheory/assets/Miscellany/日期公式.cpp"


TEST(Date, Epoch) { EXPECT_EQ(getId(1, 1, 1), 0); }

TEST(Date, RoundTrip) {
  vector<tuple<int, int, int>> days = {
      {1, 1, 1},   {1, 12, 31}, {4, 2, 29},   {100, 2, 28},
      {400, 2, 29}, {1582, 10, 15}, {1970, 1, 1}, {2000, 2, 29},
      {2024, 2, 29}, {9999, 12, 31}, {2016, 1, 31}, {2016, 3, 1}};
  for (auto [y, m, d] : days) {
    int id = getId(y, m, d);
    auto [yy, mm, dd] = date(id);
    EXPECT_EQ(yy, y) << y << "-" << m << "-" << d;
    EXPECT_EQ(mm, m);
    EXPECT_EQ(dd, d);
  }
  for (int y = 1; y <= 2400; y += 37) {
    for (int m = 1; m <= 12; ++m) {
      int md = 28;
      if (m == 2) {
        bool leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        md = leap ? 29 : 28;
      } else if (m == 4 || m == 6 || m == 9 || m == 11)
        md = 30;
      else
        md = 31;
      for (int d : {1, md / 2, md}) {
        int id = getId(y, m, d);
        auto [yy, mm, dd] = date(id);
        EXPECT_EQ(std::make_tuple(yy, mm, dd), std::make_tuple(y, m, d));
      }
    }
  }
}

TEST(Date, WeekdayKnown) {
  // 2000-01-01 is Saturday. getId(1,1,1)=0 and weekday=(id+1)%7, Sun=0.
  int id = getId(2000, 1, 1);
  EXPECT_EQ((id + 1) % 7, 6);
}
