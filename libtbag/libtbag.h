/**
 * @file   libtbag.h
 * @brief  libtbag entry-point.
 * @author zer0
 * @date   2016-11-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIBTBAG_H__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIBTBAG_H__

/* MS compatible compilers support #pragma once */
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#define AB_TRUE   1
#define AB_FALSE  0

#ifndef LOCALE_NAME_MAX_LENGTH
#define LOCALE_NAME_MAX_LENGTH 85 // Win32 API.
#endif

#ifndef NULL
#define NULL 0
#endif

typedef int tbBOOL;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * tbag initialize paramer.
 */
struct tbInitParam
{
    char localname[LOCALE_NAME_MAX_LENGTH];
};

TBAG_API int tbGetMajorVersion();
TBAG_API int tbGetMinorVersion();
TBAG_API int tbGetPatchVersion();

TBAG_API tbBOOL tbInitialize(tbInitParam * param = NULL);
TBAG_API tbBOOL tbRelease();

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* __INCLUDE_LIBTBAG__LIBTBAG_LIBTBAG_H__ */

