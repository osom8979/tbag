/**
 * @file   SysInfo.cpp
 * @brief  SysInfo class implementation.
 * @author zer0
 * @date   2018-01-08
 */

#include <libtbag/system/SysInfo.hpp>
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

