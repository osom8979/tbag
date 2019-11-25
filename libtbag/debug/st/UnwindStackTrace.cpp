/**
 * @file   UnwindStackTrace.cpp
 * @brief  UnwindStackTrace class implementation.
 * @author zer0
 * @date   2017-06-06
 */

#include <libtbag/debug/st/UnwindStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

extern "C" {
#if defined(TBAG_HAVE_UNWIND_H)
#include <unwind.h>
#endif
}

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

struct UnwindState
{
    void ** current;
    void ** end;
};

bool isUnwindSupport() TBAG_NOEXCEPT
{
#if defined(TBAG_HAVE_UNWIND_H)
    return true;
#else
    return false;
#endif
}

#if defined(TBAG_HAVE_UNWIND_H)
static _Unwind_Reason_Code __unwind_callback__(struct _Unwind_Context * context, void * arg)
{
    UnwindState * state = static_cast<UnwindState*>(arg);
    *state->current = reinterpret_cast<void*>(_Unwind_GetIP(context));
    ++state->current;
    if (!*(state->current - 1) || state->current == state->end) {
        return _URC_END_OF_STACK;
    }
    return _URC_NO_REASON;
}
#endif

std::vector<void*> getUnwindStackTrace(int max_depth)
{
    std::vector<void*> result;
#if defined(TBAG_HAVE_UNWIND_H)
    std::vector<void*> stack(max_depth);
    UnwindState state = { stack.data(), stack.data() + max_depth };
    _Unwind_Backtrace(&__unwind_callback__, &state);
    std::size_t const SIZE = state.current - stack.data();
    result.assign(stack.begin(), stack.begin() + SIZE);
#endif
    return result;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

