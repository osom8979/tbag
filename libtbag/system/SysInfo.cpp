/**
 * @file   SysInfo.cpp
 * @brief  SysInfo class implementation.
 * @author zer0
 * @date   2018-01-08
 */

#include <libtbag/system/SysInfo.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/config-ex.h>
#include <libtbag/Unit.hpp>

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
#if defined(HAVE_SYS_STATVFS_H)
#include <sys/statvfs.h>
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

bool getFilesystemInfo(std::string const & path, FilesystemStatistics & result)
{
    result.clear();
#if defined(TBAG_PLATFORM_WINDOWS)
    ULONGLONG total_byte = 0;
    ULONGLONG  free_byte = 0;
    if (::GetDiskFreeSpaceExA(path.c_str(), (PULARGE_INTEGER)&free_byte, (PULARGE_INTEGER)&total_byte, NULL) != 0) {
        result.namemax    = FILENAME_MAX;
        result.total_byte = total_byte;
        result.free_byte  = free_byte;
        result.used_byte  = total_byte - free_byte;
        return true;
    }
#elif defined(HAVE_SYS_STATVFS_H)
    struct statvfs buffer = {0,};
    if (::statvfs(path.c_str(), &buffer) == 0) {
        result.bsize      = buffer.f_bsize;
        result.frsize     = buffer.f_frsize;
        result.blocks     = buffer.f_blocks;
        result.bfree      = buffer.f_bfree;
        result.bavail     = buffer.f_bavail;
        result.files      = buffer.f_files;
        result.ffree      = buffer.f_ffree;
        result.favail     = buffer.f_favail;
        result.fsid       = buffer.f_fsid;
        result.flag       = buffer.f_flag;
        result.namemax    = buffer.f_namemax;
        result.total_byte = buffer.f_blocks * buffer.f_frsize;
        result.free_byte  = buffer.f_bavail * buffer.f_frsize;
        result.used_byte  = (buffer.f_blocks - buffer.f_bavail) * buffer.f_frsize;
        return true;
    }
#endif
    return false;
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

