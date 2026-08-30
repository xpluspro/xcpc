#pragma once

#include <gtest/gtest.h>

// Contest templates use C assert(). A guard test PASSES only if the process
// aborts. A missing abort is a CI failure (the template failed to catch misuse).
// Message text differs between libstdc++ and Apple libc; match both.
#define XCPC_EXPECT_ASSERT(stmt) \
  EXPECT_DEATH({ stmt; }, "Assertion|assertion|failed|SIGABRT")
