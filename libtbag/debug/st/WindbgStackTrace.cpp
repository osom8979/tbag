/**
 * @file   WindbgStackTrace.cpp
 * @brief  WindbgStackTrace class implementation.
 * @author zer0
 * @date   2017-06-06
 */

#include <libtbag/debug/st/WindbgStackTrace.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#if defined(HAVE_DBGENG_CO_INIT)
#include <windows.h>
#include <Dbgeng.h>
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Dbgeng.lib")
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

bool isWindbgSupport() TBAG_NOEXCEPT
{
#if defined(HAVE_DBGENG_CO_INIT)
    return true;
#else
    return false;
#endif
}

std::vector<void*> getWindbgStackTrace(int max_depth)
{
    std::vector<void*> result;
#if defined(HAVE_DBGENG_CO_INIT)
#endif
    return result;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

