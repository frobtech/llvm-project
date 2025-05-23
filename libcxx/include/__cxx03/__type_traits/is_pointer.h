//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___CXX03___TYPE_TRAITS_IS_POINTER_H
#define _LIBCPP___CXX03___TYPE_TRAITS_IS_POINTER_H

#include <__cxx03/__config>
#include <__cxx03/__type_traits/integral_constant.h>
#include <__cxx03/__type_traits/remove_cv.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if __has_builtin(__is_pointer)

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_pointer : _BoolConstant<__is_pointer(_Tp)> {};

#else // __has_builtin(__is_pointer)

template <class _Tp>
struct __libcpp_is_pointer : public false_type {};
template <class _Tp>
struct __libcpp_is_pointer<_Tp*> : public true_type {};

template <class _Tp>
struct __libcpp_remove_objc_qualifiers {
  typedef _Tp type;
};
#  if defined(_LIBCPP_HAS_OBJC_ARC)
// clang-format off
template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __strong> { typedef _Tp type; };
template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __weak> { typedef _Tp type; };
template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __autoreleasing> { typedef _Tp type; };
template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __unsafe_unretained> { typedef _Tp type; };
// clang-format on
#  endif

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_pointer
    : public __libcpp_is_pointer<typename __libcpp_remove_objc_qualifiers<__remove_cv_t<_Tp> >::type> {};

#endif // __has_builtin(__is_pointer)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___CXX03___TYPE_TRAITS_IS_POINTER_H
