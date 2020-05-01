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
#if defined(TBAG_HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if defined(TBAG_HAVE_SYS_SYSCTL_H)
#include <sys/sysctl.h>
#endif
#if defined(TBAG_HAVE_SYS_STATVFS_H)
#include <sys/statvfs.h>
#endif

#include <uv.h>

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
#elif defined(TBAG_HAVE_UNISTD_H)
    return static_cast<int>(sysconf(_SC_PAGESIZE));
#else
    return 0;
#endif
}

int getCacheLineSize(int cpu_index)
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
#elif (defined(TBAG_PLATFORM_APPLE) && defined(TBAG_HAVE_SYS_SYSCTL_H))
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
#elif defined(TBAG_HAVE_SYS_STATVFS_H)
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

std::size_t getResidentSetMemory()
{
    std::size_t rss = 0;
    int const CODE = ::uv_resident_set_memory(&rss);
    if (CODE != 0) {
        tDLogE("getResidentSetMemory() {} error", getUvErrorName(CODE));
    }
    return rss;
}

double getUptime()
{
    double uptime = 0.0f;
    int const CODE = ::uv_uptime(&uptime);
    if (CODE != 0) {
        tDLogE("getUptime() {} error", getUvErrorName(CODE));
    }
    return uptime;
}

std::vector<CpuInfo> getCpuInfos()
{
    uv_cpu_info_t * infos = nullptr;
    int count = 0;

    // Gets information about the CPUs on the system.
    // The cpu_infos array will have count elements and needs to be freed with uv_free_cpu_info().
    int const CODE = ::uv_cpu_info(&infos, &count);
    if (convertUvErrorToErrWithLogging("getCpuInfos()", CODE) != E_SUCCESS) {
        return std::vector<CpuInfo>();
    }

    assert(count > 0);
    std::vector<CpuInfo> result(static_cast<std::size_t>(count));

    uv_cpu_info_t * cursor = infos;
    for (int i = 0; i < count; ++i, ++cursor) {
        result[i].model.assign(cursor->model);
        result[i].speed = cursor->speed;
        result[i].times.user = cursor->cpu_times.user;
        result[i].times.nice = cursor->cpu_times.nice;
        result[i].times.sys  = cursor->cpu_times.sys ;
        result[i].times.idle = cursor->cpu_times.idle;
        result[i].times.irq  = cursor->cpu_times.irq ;
    }

    // Frees the cpu_infos array previously allocated with uv_cpu_info().
    ::uv_free_cpu_info(infos, count);

    return result;
}

std::vector<InterfaceAddress> getInterfaceAddresses()
{
    uv_interface_address_t * infos = nullptr;
    int count = 0;

    // Gets address information about the network interfaces on the system.
    // An array of count elements is allocated and returned in addresses.
    // It must be freed by the user, calling uv_free_interface_addresses().
    int const CODE = uv_interface_addresses(&infos, &count);
    if (convertUvErrorToErrWithLogging("getInterfaceAddresses()", CODE) != E_SUCCESS) {
        return std::vector<InterfaceAddress>();
    }

    assert(count >= 0);
    std::vector<InterfaceAddress> result(static_cast<std::size_t>(count));

    uv_interface_address_t * cursor = infos;
    for (int i = 0; i < count; ++i, ++cursor) {
        result[i].name.assign(cursor->name);

        static_assert(PHYSICAL_ADDRESS_SIZE == sizeof(cursor->phys_addr),
                      "The sizes of PHYSICAL_ADDRESS_SIZE and uv_interface_address_s.phys_addr should be the same.");
        for (int pi = 0; pi < result[i].physical_address.max_size(); ++pi) {
            result[i].physical_address[pi] = cursor->phys_addr[pi];
        }

        result[i].is_internal = (cursor->is_internal != 0 ? true : false);

        static_assert(sizeof(result[i].address) == sizeof(cursor->address),
                      "The sizes of InterfaceAddress.address and uv_interface_address_s.address should be the same.");
        static_assert(sizeof(result[i].netmask) == sizeof(cursor->netmask),
                      "The sizes of InterfaceAddress.netmask and uv_interface_address_s.netmask should be the same.");
        static_assert(sizeof(SocketAddress) == sizeof(cursor->address) && sizeof(SocketAddress) == sizeof(cursor->netmask),
                      "The sizes of SocketAddress and address and netmask should be the same.");

        ::memcpy(&result[i].address, &cursor->address, sizeof(cursor->address));
        ::memcpy(&result[i].netmask, &cursor->netmask, sizeof(cursor->netmask));

        assert(result[i].address.in4.sin_family == result[i].address.in6.sin6_family);
        assert(result[i].netmask.in4.sin_family == result[i].netmask.in6.sin6_family);
    }

    // Free an array of uv_interface_address_t which was returned by uv_interface_addresses().
    ::uv_free_interface_addresses(infos, count);

    return result;
}

std::string convertPhysicalToString(PhysicalAddress const & physical)
{
    std::vector<uint8_t> const BUFFER(physical.begin(), physical.end());
    return libtbag::string::convertByteVectorToHexString(BUFFER, std::string(), std::string(":"));
}

Err changeDirectory(std::string const & dir)
{
    int const code = ::uv_chdir(dir.c_str());
    if (code != 0) {
        tDLogE("changeDirectory() {} error", getUvErrorName(code));
    }
    return convertUvErrorToErr(code);
}

std::vector<double> getLoadAverage()
{
    std::size_t const SIZE = 3;
    double avg[SIZE] = {0,};
    ::uv_loadavg(avg);
    return std::vector<double>({avg[0], avg[1], avg[2]});
}

Password getPassword()
{
    // The populated data includes the username, euid, gid, shell, and home directory.
    // On non-Windows systems, all data comes from getpwuid_r(3).
    // On Windows, uid and gid are set to -1 and have no meaning, and shell is NULL.
    // After successfully calling this function,
    // the memory allocated to pwd needs to be freed with uv_os_free_passwd().

    uv_passwd_t pwd = {0,};
    int const CODE = ::uv_os_get_passwd(&pwd);
    if (CODE != 0) {
        tDLogE("getPassword() {} error", getUvErrorName(CODE));
        return Password{};
    }

    Password result;
    if (pwd.username != nullptr) {
        result.username.assign(pwd.username);
    }
    if (pwd.shell != nullptr) {
        result.shell.assign(pwd.shell);
    }
    if (pwd.homedir != nullptr) {
        result.homedir.assign(pwd.homedir);
    }
    result.uid = pwd.uid;
    result.gid = pwd.gid;

    // Frees the pwd memory previously allocated with uv_os_get_passwd().
    ::uv_os_free_passwd(&pwd);

    return result;
}

uint64_t getTotalMemory()
{
    return ::uv_get_total_memory();
}

uint64_t getHighResolutionTime()
{
    // It is relative to an arbitrary time in the past.
    // It is not related to the time of day and therefore not subject to clock drift.
    // The primary use is for measuring performance between intervals.
    //
    // Not every platform can support nanosecond resolution;
    // however, this value will always be in nanoseconds.
    return ::uv_hrtime();
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

