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
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    BOOL const FIRST_CODE = ::GetLogicalProcessorInformation(buffer, &length);
    if (FIRST_CODE == FALSE && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)::malloc(length);
    } else {
        tDLogE("getCacheLineSize() Insufficient buffer error.");
        return 0;
    }

    if (::GetLogicalProcessorInformation(buffer, &length) == TRUE) {
        for (int i = 0; i != length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
            if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
                line_size = (std::size_t)buffer[i].Cache.LineSize;
                break;
            }
        }
    } else {
        tDLogE("getCacheLineSize() Error code: {}", ::GetLastError());
    }

    if (buffer != NULL) {
        ::free(buffer);
    }
#elif (defined(TBAG_PLATFORM_APPLE) && defined(HAVE_SYS_SYSCTL_H))
    std::size_t sizeof_line_size = sizeof(line_size);
    if (::sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, nullptr, 0) != 0) {
        return 0;
    }
#elif defined(TBAG_PLATFORM_LINUX)
    FILE * f = ::fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
    if (f == nullptr) {
        return 0;
    }
    int content = 0;
    ::fscanf(f, "%d", &content);
    ::fclose(f);
    line_size = (std::size_t)content;
#endif
    return line_size;
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

