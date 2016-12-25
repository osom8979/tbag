/**
 * @file   StackTrace.hpp
 * @brief  StackTrace class prototype.
 * @author zer0
 * @date   2016-12-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_STACKTRACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_STACKTRACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

TBAG_API std::string getStackTrace();

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_STACKTRACE_HPP__

