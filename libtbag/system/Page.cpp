/**
 * @file   Page.cpp
 * @brief  Page class implementation.
 * @author zer0
 * @date   2017-10-19
 */

#include <libtbag/system/Page.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#if defined(TBAG_PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

int getPageSize()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    SYSTEM_INFO info = {0,};
    GetSystemInfo(&info);
    return static_cast<int>(info.dwPageSize);
#elif defined(HAVE_UNISTD_H)
    return static_cast<int>(sysconf(_SC_PAGESIZE));
#else
    return 0;
#endif
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

