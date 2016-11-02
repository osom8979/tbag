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

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_EXPORTS int tbGetMajorVersion();
TBAG_EXPORTS int tbGetMinorVersion();
TBAG_EXPORTS int tbGetPatchVersion();

TBAG_EXPORTS int tbInitialize();

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* __INCLUDE_LIBTBAG__LIBTBAG_LIBTBAG_H__ */

