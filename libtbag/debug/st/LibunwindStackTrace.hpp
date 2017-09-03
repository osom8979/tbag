/**
 * @file   LibunwindStackTrace.hpp
 * @brief  LibunwindStackTrace class prototype.
 * @author zer0
 * @date   2017-09-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_LIBUNWINDSTACKTRACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_LIBUNWINDSTACKTRACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

TBAG_API bool isLibunwindSupport() TBAG_NOEXCEPT;
TBAG_API std::vector<void*> getLibunwindStackTrace(int max_depth = 64);

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_LIBUNWINDSTACKTRACE_HPP__

