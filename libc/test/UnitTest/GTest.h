//===-- Header for using the gtest framework --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_TEST_UNITTEST_GTEST_H
#define LLVM_LIBC_TEST_UNITTEST_GTEST_H

#include "src/__support/CPP/string.h"
#include "src/__support/CPP/string_view.h"
#include "src/__support/big_int.h"
#include "src/__support/c_string.h"
#include "src/__support/integer_to_string.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"
#include "src/__support/uint128.h"
#include "test/UnitTest/TestLogger.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ostream>

// System <ctype.h> (transitively included via <ostream>) may define XSI
// function-like macros that libc++ <cctype> does not undefine, which would
// otherwise expand inside qualified LIBC_NAMESPACE::isascii / toascii calls.
#undef isascii
#undef toascii

#ifdef libc_make_test_file_path
#undef libc_make_test_file_path
#endif
#define libc_make_test_file_path(file_name)                                    \
  (LIBC_NAMESPACE::testing::libc_make_test_file_path_func(file_name))

namespace LIBC_NAMESPACE_DECL {
namespace cpp {

LIBC_INLINE ::std::ostream &operator<<(::std::ostream &os, string_view sv) {
  return os.write(sv.data(), static_cast<::std::streamsize>(sv.size()));
}

LIBC_INLINE ::std::ostream &operator<<(::std::ostream &os, const string &s) {
  return os.write(s.data(), static_cast<::std::streamsize>(s.size()));
}

LIBC_INLINE void PrintTo(string_view sv, ::std::ostream *os) { *os << sv; }

LIBC_INLINE void PrintTo(const string &s, ::std::ostream *os) { *os << s; }

} // namespace cpp

template <size_t Bits, bool Signed, typename WordType>
LIBC_INLINE ::std::ostream &
operator<<(::std::ostream &os, const BigInt<Bits, Signed, WordType> &val) {
  const IntegerToString<BigInt<Bits, Signed, WordType>, radix::Hex::WithPrefix>
      buffer(val);
  return os << buffer.view();
}

template <size_t Bits, bool Signed, typename WordType>
LIBC_INLINE void PrintTo(const BigInt<Bits, Signed, WordType> &val,
                         ::std::ostream *os) {
  *os << val;
}

namespace testing {

using ::testing::MatchResultListener;
using ::testing::Test;
using ::testing::Types;

enum class TestCond { EQ, NE, LT, LE, GT, GE };

struct MatcherBase {
  using is_gtest_matcher = void;

  LIBC_INLINE void DescribeTo(::std::ostream *os) const { *os << "matches"; }

  LIBC_INLINE void DescribeNegationTo(::std::ostream *os) const {
    *os << "does not match";
  }
};

template <typename T> struct Matcher : public MatcherBase {};

CString libc_make_test_file_path_func(const char *file_name);

} // namespace testing
} // namespace LIBC_NAMESPACE_DECL

#define LIBC_TEST_SKIP_CALLABLE_(FUNC, REASON)                                 \
  do {                                                                         \
    if constexpr (false) {                                                     \
      (FUNC)();                                                                \
    }                                                                          \
    GTEST_SKIP() << (REASON);                                                  \
  } while (false)

#define LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, ARG, REASON)                        \
  do {                                                                         \
    if constexpr (false) {                                                     \
      (FUNC)();                                                                \
      (void)(ARG);                                                             \
    }                                                                          \
    GTEST_SKIP() << (REASON);                                                  \
  } while (false)

#if defined(GTEST_HAS_DEATH_TEST) && GTEST_HAS_DEATH_TEST &&                   \
    (!defined(LIBC_TEST_SUBPROCESS_TESTS) || LIBC_TEST_SUBPROCESS_TESTS)

#define LIBC_TEST_EXPECT_EXITS(FUNC, CODE)                                     \
  EXPECT_EXIT((FUNC)(), ::testing::ExitedWithCode(CODE), "")
#define LIBC_TEST_ASSERT_EXITS(FUNC, CODE)                                     \
  ASSERT_EXIT((FUNC)(), ::testing::ExitedWithCode(CODE), "")

#ifdef LIBC_TEST_SKIP_DEATH_TESTS

#define LIBC_TEST_EXPECT_TRAP(FUNC)                                            \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_ASSERT_TRAP(FUNC)                                            \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_EXPECT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_ASSERT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_EXPECT_PROCESS_CRASH(FUNC)                                   \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_ASSERT_PROCESS_CRASH(FUNC)                                   \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_EXPECT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG, "LIBC_TEST_SKIP_DEATH_TESTS")
#define LIBC_TEST_ASSERT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG, "LIBC_TEST_SKIP_DEATH_TESTS")

#else // !LIBC_TEST_SKIP_DEATH_TESTS

#define LIBC_TEST_EXPECT_TRAP(FUNC) EXPECT_DEATH((FUNC)(), "")
#define LIBC_TEST_ASSERT_TRAP(FUNC) ASSERT_DEATH((FUNC)(), "")
#define LIBC_TEST_EXPECT_ASSERTION_FAILURE(FUNC) EXPECT_DEATH((FUNC)(), "")
#define LIBC_TEST_ASSERT_ASSERTION_FAILURE(FUNC) ASSERT_DEATH((FUNC)(), "")
#define LIBC_TEST_EXPECT_PROCESS_CRASH(FUNC) EXPECT_DEATH((FUNC)(), "")
#define LIBC_TEST_ASSERT_PROCESS_CRASH(FUNC) ASSERT_DEATH((FUNC)(), "")

#if !defined(GTEST_OS_FUCHSIA) && !defined(GTEST_OS_WINDOWS)
#define LIBC_TEST_EXPECT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  EXPECT_EXIT((FUNC)(), ::testing::KilledBySignal(SIG), "")
#define LIBC_TEST_ASSERT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  ASSERT_EXIT((FUNC)(), ::testing::KilledBySignal(SIG), "")
#else
#define LIBC_TEST_EXPECT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG,                                      \
                               "POSIX signals unavailable on this OS")
#define LIBC_TEST_ASSERT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG,                                      \
                               "POSIX signals unavailable on this OS")
#endif

#endif // LIBC_TEST_SKIP_DEATH_TESTS

#else // !GTEST_HAS_DEATH_TEST || !LIBC_TEST_SUBPROCESS_TESTS

#define LIBC_TEST_EXPECT_EXITS(FUNC, CODE)                                     \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, CODE, "subprocess tests disabled")
#define LIBC_TEST_ASSERT_EXITS(FUNC, CODE)                                     \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, CODE, "subprocess tests disabled")
#define LIBC_TEST_EXPECT_TRAP(FUNC)                                            \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_ASSERT_TRAP(FUNC)                                            \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_EXPECT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_ASSERT_ASSERTION_FAILURE(FUNC)                               \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_EXPECT_PROCESS_CRASH(FUNC)                                   \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_ASSERT_PROCESS_CRASH(FUNC)                                   \
  LIBC_TEST_SKIP_CALLABLE_(FUNC, "subprocess tests disabled")
#define LIBC_TEST_EXPECT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG, "subprocess tests disabled")
#define LIBC_TEST_ASSERT_KILLED_BY_SIGNAL(FUNC, SIG)                           \
  LIBC_TEST_SKIP_CALLABLE_ARG_(FUNC, SIG, "subprocess tests disabled")

#endif

#define LIBC_TEST_HAS_MATCHERS() (1)

#endif // LLVM_LIBC_TEST_UNITTEST_GTEST_H
