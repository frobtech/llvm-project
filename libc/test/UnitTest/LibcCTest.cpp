//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Helpers for libc C unittests.
///
//===----------------------------------------------------------------------===//

#include "test/UnitTest/LibcCTest.h"
#include "test/UnitTest/Test.h"

#ifdef LIBC_COPT_TEST_USE_GTEST

TEST(LlvmLibcCTest, c_test) { libc_c_test_run(); }

extern "C" void libc_c_test_anchor() {}

extern "C" void libc_c_test_fail(const char *cond, int expected,
                                 const char *file, int line) {
  ADD_FAILURE_AT(file, line)
      << "Expected (" << cond << ") to be " << (expected ? "true" : "false");
}

#else

namespace {
class CTest : public LIBC_NAMESPACE::testing::Test {
public:
  CTest() { addTest(this); }
  void Run() override { libc_c_test_run(); }
  const char *getName() const override { return LIBC_C_TEST_NAME; }
};
} // namespace

static CTest c_test_instance;

extern "C" void libc_c_test_anchor() {}

extern "C" void libc_c_test_fail(const char *cond, int expected,
                                 const char *file, int line) {
  LIBC_NAMESPACE::testing::internal::test(
      LIBC_NAMESPACE::testing::TestCond::EQ, !expected,
      static_cast<bool>(expected), cond, expected ? "true" : "false",
      LIBC_NAMESPACE::testing::internal::Location(file, line));
}

#endif
