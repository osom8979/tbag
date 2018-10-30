/**
 * @file   libtbui.hpp
 * @brief  libtbui class prototype.
 * @author zer0
 * @date   2018-10-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_LIBTBUI_H__
#define __INCLUDE_LIBTBAG__LIBTBUI_LIBTBUI_H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbui/export.h>

#if defined(__cplusplus)
extern "C" {
#endif

TBUI_API int tbuiGetMajorVersion();
TBUI_API int tbuiGetMinorVersion();

TBUI_API int tbuiExternalMain(int argc, char ** argv, char ** envs);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBUI_LIBTBUI_H__

