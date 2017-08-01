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

#ifndef TB_TRUE
#define TB_TRUE 1
#endif

#ifndef TB_FALSE
#define TB_FALSE  0
#endif

#ifndef TB_LOCALE_NAME_MAX_LENGTH
#define TB_LOCALE_NAME_MAX_LENGTH 85 // Win32 API.
#endif

#ifndef TB_NULL
#define TB_NULL 0
#endif

typedef int tbBOOL;

/**
 * tbag initialize paramer.
 */
struct tbInitParam
{
    char localname[TB_LOCALE_NAME_MAX_LENGTH];
};

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API int tbGetMajorVersion();
TBAG_API int tbGetMinorVersion();
TBAG_API int tbGetPatchVersion();

TBAG_API tbBOOL tbInitialize(tbInitParam * param = TB_NULL);
TBAG_API tbBOOL tbRelease();

TBAG_API int tbPotMain(int argc, char ** argv, char ** envs = TB_NULL);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* __INCLUDE_LIBTBAG__LIBTBAG_LIBTBAG_H__ */

