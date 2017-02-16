/**
 * @file   tbshare.h
 * @brief  tbshare represent header.
 * @author zer0
 * @date   2016-04-17
 * @date   2016-10-14 (Rename: libtbag_test -> libtbtest)
 * @date   2016-10-25 (Rename: libtbtest -> tbshare)
 */

#ifndef __INCLUDE_LIBTBAG__TEST_TBSHARE_TBSHARE_H__
#define __INCLUDE_LIBTBAG__TEST_TBSHARE_TBSHARE_H__

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

/** Add function test. */
LIBTBAG_TEST_EXPORT int tbshare_add(int v1, int v2);

/** Add function test2. */
LIBTBAG_TEST_EXPORT void tbshare_add2(int v1, int v2, int * result);

#if defined(__cplusplus)
} /** extern "C" */
#endif

#endif /** __INCLUDE_LIBTBAG__TEST_TBSHARE_TBSHARE_H__ */

