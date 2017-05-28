/**
 * @file   StackTraceInterface.hpp
 * @brief  StackTraceInterface class prototype.
 * @author zer0
 * @date   2017-05-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STACKTRACEINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STACKTRACEINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

/**
 * Interface of StackTrace.
 *
 * @author zer0
 * @date   2017-05-28
 */
struct StackTraceInterface
{
    virtual std::string name() const = 0;
    virtual std::string source() const = 0;
    virtual std::size_t line() const = 0;
};

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STACKTRACEINTERFACE_HPP__

