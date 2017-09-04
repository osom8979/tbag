/**
 * @file   ExecInfoStackTrace.cpp
 * @brief  ExecInfoStackTrace class implementation.
 * @author zer0
 * @date   2017-09-03
 */

#include <libtbag/debug/st/ExecInfoStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#include <cstring>
#include <iterator>

#if defined(HAVE_EXECINFO_H)
#include <execinfo.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

bool isExecInfoSupport() TBAG_NOEXCEPT
{
#if defined(HAVE_EXECINFO_H)
    return true;
#else
    return false;
#endif
}

std::vector<void*> getExecInfoStackTrace(int max_depth)
{
    std::vector<void*> result;
#if defined(HAVE_EXECINFO_H)
    std::vector<void*> stack(static_cast<std::size_t>(max_depth));
    int const SIZE = ::backtrace(stack.data(), max_depth);
    result.assign(stack.begin(), stack.begin() + SIZE);
#endif
    return result;
}

std::vector<std::string> getExecInfoSymbolize(std::vector<void*> const & stack)
{
    std::vector<std::string> result;
#if defined(HAVE_EXECINFO_H)
    int const SIZE = static_cast<int>(stack.size());
    char ** strings = ::backtrace_symbols(stack.data(), SIZE);
    if (strings != nullptr) {
        for (int i = 0; i < SIZE; i++) {
            result.emplace_back(strings[i]);
        }
    }
#endif
    return result;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

