/**
 * @file   ExecInfoStackTrace.hpp
 * @brief  ExecInfoStackTrace class prototype.
 * @author zer0
 * @date   2017-09-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_EXECINFOSTACKTRACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_EXECINFOSTACKTRACE_HPP__

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

TBAG_API bool isExecInfoSupport() TBAG_NOEXCEPT;

TBAG_API std::vector<void*> getExecInfoStackTrace(int max_depth = DEFAULT_MAX_DEPTH_OF_STACKTRACE);
TBAG_API std::vector<std::string> getExecInfoSymbolize(std::vector<void*> const & stack);

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_EXECINFOSTACKTRACE_HPP__

