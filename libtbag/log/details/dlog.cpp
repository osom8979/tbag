/**
 * @file   dlog.cpp
 * @brief  dlog class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/log/details/dlog.hpp>
#include <libtbag/lock/UvLock.hpp>

#include <cstdio>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

void dlog(char const * TBAG_RESTRICT text)
{
    assert(text != nullptr);
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    static libtbag::lock::UvLock __lock__;
    __lock__.lock();
#endif
    ::fputs(text, stderr);
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    __lock__.unlock();
#endif
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

