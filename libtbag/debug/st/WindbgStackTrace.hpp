/**
 * @file   WindbgStackTrace.hpp
 * @brief  WindbgStackTrace class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_WINDBGSTACKTRACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_WINDBGSTACKTRACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/st/StFrame.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

TBAG_API bool isWindbgSupport() TBAG_NOEXCEPT;
TBAG_API std::vector<void*> getWindbgStackTrace(int skip = 0, int max_depth = DEFAULT_MAX_DEPTH_OF_STACKTRACE);

// clang-format off
TBAG_API std::string getWindbgName  (void const * addr);
TBAG_API std::string getWindbgSource(void const * addr);
TBAG_API std::size_t getWindbgLine  (void const * addr);
// clang-format on

TBAG_API std::vector<std::string> getWindbgSymbolize(std::vector<void*> const & stack);

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_WINDBGSTACKTRACE_HPP__

