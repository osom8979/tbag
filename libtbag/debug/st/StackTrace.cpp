/**
 * @file   StackTrace.cpp
 * @brief  StackTrace class implementation.
 * @author zer0
 * @date   2017-06-06
 */

#include <libtbag/debug/st/StackTrace.hpp>
#include <libtbag/log/Log.hpp>

#if defined(TBAG_STACKTRACE_WINDBG)
#elif defined(TBAG_STACKTRACE_UNWIND) || defined(TBAG_STACKTRACE_BACKTRACE)
#else
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

StackTrace::StackTrace()
{
    // EMPTY.
}

StackTrace::~StackTrace()
{
    // EMPTY.
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

