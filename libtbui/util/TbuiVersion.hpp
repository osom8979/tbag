/**
 * @file   TbuiVersion.hpp
 * @brief  TbuiVersion class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_UTIL_TBUIVERSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_UTIL_TBUIVERSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/Version.hpp>

#include <libtbui/export.h>

namespace libtbui {
namespace util    {

using Version = libtbag::util::Version;

TBUI_API int getMajorVersion();
TBUI_API int getMinorVersion();

TBUI_API Version getVersion();

} // namespace util
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_UTIL_TBUIVERSION_HPP__

