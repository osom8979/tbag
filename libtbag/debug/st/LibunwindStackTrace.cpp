/**
 * @file   LibunwindStackTrace.cpp
 * @brief  LibunwindStackTrace class implementation.
 * @author zer0
 * @date   2017-09-03
 */

#include <libtbag/debug/st/LibunwindStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#include <atomic>

extern "C" {
#if defined(HAVE_LIBUNWIND_H)
#include <libunwind.h>
#endif
}

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

bool isLibunwindSupport() TBAG_NOEXCEPT
{
#if defined(HAVE_LIBUNWIND_H)
    return true;
#else
    return false;
#endif
}

// Sometimes, we can try to get a stack trace from within a stack
// trace, because libunwind can call mmap (maybe indirectly via an
// internal mmap based memory allocator), and that mmap gets trapped
// and causes a stack-trace request.  If were to try to honor that
// recursive request, we'd end up with infinite recursion or deadlock.
// Luckily, it's safe to ignore those subsequent traces.  In such
// cases, we return 0 to indicate the situation.
static std::atomic_bool g_now_entering(false);

std::vector<void*> getLibunwindStackTrace(int max_depth)
{
    std::vector<void*> result;
#if defined(HAVE_LIBUNWIND_H)
    bool expected = false;
    if (g_now_entering.compare_exchange_strong(expected, true) == false) {
        return result;
    }

    unw_context_t uc;
    unw_cursor_t cursor;

    // On successful completion, unw_getcontext() returns 0.
    // Otherwise, a value of -1 is returned.
    if (::unw_getcontext(&uc) != 0) {
        tDLogE("getUnwindStackTrace() unw_getcontext failed.");
        g_now_entering = false;
        return result;
    }

    // On successful completion, unw_init_local() returns 0.
    // Otherwise the negative value of one of the error-codes below is returned.
    if (::unw_init_local(&cursor, &uc) != 0) {
        tDLogE("getUnwindStackTrace() unw_init_local failed.");
        g_now_entering = false;
        return result;
    }

    void * ip = nullptr;
    int ret = 0;
    int i = 0;

    while (i < max_depth) {
        ret = ::unw_get_reg(&cursor, UNW_REG_IP, (unw_word_t*) &ip);
        if (ret < 0) {
            break;
        }
        result.push_back(ip);
        ret = ::unw_step(&cursor);
        if (ret <= 0) {
            break;
        }
    }
    g_now_entering = false;
#endif
    return result;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

