#include <gtest/gtest.h>
#include "prelude/common.hpp"
#include "src/sections/DataStructures/assets/DataStructure/hashmap.cpp"


TEST(Hashmap, InsertGetClear) {
  hashmap<int, 97> h;
  EXPECT_EQ(h.get(1), 0);
  h[1] = 10;
  h[2] = 20;
  EXPECT_EQ(h.get(1), 10);
  EXPECT_EQ(h[2], 20);
  h[1] = 11;
  EXPECT_EQ(h.get(1), 11);
  h.clear();
  EXPECT_EQ(h.get(1), 0);
  h[3] = 7;
  EXPECT_EQ(h.get(3), 7);
  h.fullclear();
  EXPECT_EQ(h.get(3), 0);
}

TEST(Hashmap, ManyKeys) {
  hashmap<int, 97> h;
  for (int i = 1; i <= 50; ++i) h[i * 17u] = i;
  for (int i = 1; i <= 50; ++i) EXPECT_EQ(h.get(i * 17u), i);
  EXPECT_EQ(h.get(123456), 0);
}
