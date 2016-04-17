/**
 * @file   libtbag_test.h
 * @brief  libtbag_test represent header.
 * @author zer0
 * @date   2016-04-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TEST_LIBTEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TEST_LIBTEST_HPP__

/* MS compatible compilers support #pragma once */
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(WIN32) || defined(_WIN32)
# include <windows.h>
# if defined(USE_LIBTEST_EXPORT)
#  define LIBTBAG_TEST_EXPORT __declspec(dllexport)
# else
#  define LIBTBAG_TEST_EXPORT __declspec(dllimport)
# endif /* USE_LIBTESTBAG_EXPORT */
#else /* defined(WIN32) || defined(_WIN32) */
# define LIBTBAG_TEST_EXPORT
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Add function test.
 */
LIBTBAG_TEST_EXPORT int __libtbag_test__add__(int v1, int v2);

#if defined(__cplusplus)
} /** extern "C" */
#endif

#endif /** __INCLUDE_LIBTBAG__LIBTBAG_TEST_LIBTEST_HPP__ */

