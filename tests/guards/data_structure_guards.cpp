#include "prelude/common.hpp"
#include "prelude/death.hpp"
#include "src/sections/DataStructures/assets/DataStructure/hashmap.cpp"

TEST(Guards, HashmapFullTableAborts) {
  hashmap<int, 5> h;
  h.fullclear();
  // 5 slots; filling all of them then inserting a new key must abort
  for (int i = 0; i < 5; ++i) h[i + 1] = i;
  XCPC_EXPECT_ASSERT((void)h[99]);
}
