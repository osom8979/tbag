/**
 * @file   UvUtils.cpp
 * @brief  uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

char ** setupArgs(int argc, char ** argv)
{
    return ::uv_setup_args(argc, argv);
}

std::string getProcessTitle()
{
    std::size_t const BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE] = {0,};

    // If buffer is NULL or size is zero, UV_EINVAL is returned.
    // If size cannot accommodate the process title and terminating NULL character,
    // the function returns UV_ENOBUFS.
    int const CODE = ::uv_get_process_title(buffer, BUFFER_SIZE);
    if (CODE != 0) {
        tDLogE("getProcessTitle() {} error", getUvErrorName(CODE));
        return std::string();
    }
    return std::string(buffer);
}

void setProcessTitle(std::string const & title)
{
    // On platforms with a fixed size buffer for the process title
    // the contents of title will be copied to the buffer
    // and truncated if larger than the available space.
    // Other platforms will return UV_ENOMEM if they cannot allocate
    // enough space to duplicate the contents of title.
    int const CODE = ::uv_set_process_title(title.c_str());
    if (CODE != 0) {
        tDLogE("setProcessTitle() {} error", getUvErrorName(CODE));
    }
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

ResourceUsage getResourceUsage()
{
    uv_rusage_t rusage = {0,};
    int const CODE = ::uv_getrusage(&rusage);
    if (CODE != 0) {
        tDLogE("getResourceUsage() {} error", getUvErrorName(CODE));
    }

    ResourceUsage result;
    result.utime.sec  = rusage.ru_utime.tv_sec;
    result.utime.usec = rusage.ru_utime.tv_usec;
    result.stime.sec  = rusage.ru_stime.tv_sec;
    result.stime.usec = rusage.ru_stime.tv_usec;
    result.maxrss     = rusage.ru_maxrss;
    result.ixrss      = rusage.ru_ixrss;
    result.idrss      = rusage.ru_idrss;
    result.isrss      = rusage.ru_isrss;
    result.minflt     = rusage.ru_minflt;
    result.majflt     = rusage.ru_majflt;
    result.nswap      = rusage.ru_nswap;
    result.inblock    = rusage.ru_inblock;
    result.oublock    = rusage.ru_oublock;
    result.msgsnd     = rusage.ru_msgsnd;
    result.msgrcv     = rusage.ru_msgrcv;
    result.nsignals   = rusage.ru_nsignals;
    result.nvcsw      = rusage.ru_nvcsw;
    result.nivcsw     = rusage.ru_nivcsw;
    return result;
}

std::vector<CpuInfo> getCpuInfos()
{
    uv_cpu_info_t * infos = nullptr;
    int count = 0;

    // Gets information about the CPUs on the system.
    // The cpu_infos array will have count elements and needs to be freed with uv_free_cpu_info().
    int const CODE = ::uv_cpu_info(&infos, &count);
    if (convertUvErrorToErrWithLogging("getCpuInfos()", CODE) != Err::E_SUCCESS) {
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
    if (convertUvErrorToErrWithLogging("getInterfaceAddresses()", CODE) != Err::E_SUCCESS) {
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
    return string::convertByteVectorToHexString(BUFFER, std::string(), std::string(":"));
}

void changeDirectory(std::string const & dir)
{
    int const CODE = ::uv_chdir(dir.c_str());
    if (CODE != 0) {
        tDLogE("changeDirectory() {} error", getUvErrorName(CODE));
    }
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

Err getEnv(std::string const & name, std::string & value)
{
    char * env_value = ::getenv(name.c_str());
    if (env_value != nullptr) {
        value = std::string(env_value);
        return Err::E_SUCCESS;
    }
    return Err::E_ENFOUND;
}

Err setEnv(std::string const & name, std::string const & value)
{
    tDLogE("setEnv() Function not implemented.");
    return Err::E_ENOSYS;
}

Err unsetEnv(std::string const & name)
{
    tDLogE("unsetEnv() Function not implemented.");
    return Err::E_ENOSYS;
}

std::string getHostName()
{
    tDLogE("getHostName() Function not implemented.");
    return std::string();
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

