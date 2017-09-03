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

// Example:
// 0  tester          0x0000000105f84708 _ZN7testing8UnitTest3RunEv + 408
// 1  tester          0x0000000105f60011 _Z13RUN_ALL_TESTSv + 17
// 2  tester          0x0000000105f5feb2 main + 578
// 3  libdyld.dylib   0x00007fffa12ef235 start + 1
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

