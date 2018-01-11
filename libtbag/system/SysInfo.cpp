/**
 * @file   SysInfo.cpp
 * @brief  SysInfo class implementation.
 * @author zer0
 * @date   2018-01-08
 */

#include <libtbag/system/SysInfo.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>

#include <cstdlib>
#include <cstdio>

#if defined(TBAG_PLATFORM_WINDOWS)
#include <Windows.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if defined(HAVE_SYS_SYSCTL_H)
#include <sys/sysctl.h>
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

int getCacheLineSize()
{
    std::size_t line_size = 0;
#if defined(TBAG_PLATFORM_WINDOWS)
    DWORD length = 0;
    if (::GetLogicalProcessorInformation(nullptr, &length) == FALSE) {
        tDLogE("getCacheLineSize() Error code: {}", ::GetLastError());
        return 0;
    }
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION * info = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)::malloc(length);
    if (::GetLogicalProcessorInformation(&info[0], &length) == TRUE) {
        for (int i = 0; i != length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
            if (info[i].Relationship == RelationCache && info[i].Cache.Level == 1) {
                line_size = (std::size_t)info[i].Cache.LineSize;
                break;
            }
        }
    } else {
        tDLogE("getCacheLineSize() Error code: {}", ::GetLastError());
    }
    ::free(info);
#elif (defined(TBAG_PLATFORM_APPLE) && defined(HAVE_SYS_SYSCTL_H))
    std::size_t sizeof_line_size = sizeof(line_size);
    if (::sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, nullptr, 0) != 0) {
        return 0;
    }
#elif defined(TBAG_PLATFORM_LINUX)
    FILE * f = ::fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
    if (f) {
        ::fscanf(f, "%d", &line_size);
        ::fclose(f);
    }
#endif
    return line_size;
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

