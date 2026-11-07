//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Adapter header for running llvm-libc unit tests with Pigweed's pw_unit_test.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_TEST_UNITTEST_PIGWEEDTEST_H
#define LLVM_LIBC_TEST_UNITTEST_PIGWEEDTEST_H

#include "src/__support/macros/config.h"
#include <gtest/gtest.h>

namespace LIBC_NAMESPACE_DECL {
namespace testing {

using ::testing::Test;

} // namespace testing
} // namespace LIBC_NAMESPACE_DECL

#define LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)                                      \
  do {                                                                         \
    if constexpr (false) {                                                     \
      (FUNC)();                                                                \
    }                                                                          \
    GTEST_SKIP();                                                              \
  } while (false)

#define LIBC_PIGWEED_SKIP_CALLABLE_ARG_(FUNC, ARG)                             \
  do {                                                                         \
    if constexpr (false) {                                                     \
      (FUNC)();                                                                \
      (void)(ARG);                                                             \
    }                                                                          \
    GTEST_SKIP();                                                              \
  } while (false)

#define LIBC_TEST_EXPECT_TRAP(FUNC) LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_ASSERT_TRAP(FUNC) LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_EXPECT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_ASSERT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_EXPECT_PROCESS_CRASH(FUNC) LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_ASSERT_PROCESS_CRASH(FUNC) LIBC_PIGWEED_SKIP_CALLABLE_(FUNC)
#define LIBC_TEST_EXPECT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_PIGWEED_SKIP_CALLABLE_ARG_(FUNC, SIG)
#define LIBC_TEST_ASSERT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_PIGWEED_SKIP_CALLABLE_ARG_(FUNC, SIG)
#define LIBC_TEST_EXPECT_EXITS(FUNC, CODE)                                     \
  LIBC_PIGWEED_SKIP_CALLABLE_ARG_(FUNC, CODE)
#define LIBC_TEST_ASSERT_EXITS(FUNC, CODE)                                     \
  LIBC_PIGWEED_SKIP_CALLABLE_ARG_(FUNC, CODE)

// pw_unit_test's light backend does not provide gmock-style matchers.
#define LIBC_TEST_HAS_MATCHERS() (0)

#endif // LLVM_LIBC_TEST_UNITTEST_PIGWEEDTEST_H
