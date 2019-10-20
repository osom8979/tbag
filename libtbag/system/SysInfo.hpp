/**
 * @file   SysInfo.hpp
 * @brief  SysInfo class prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <array>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

TBAG_API int getPageSize();
TBAG_API int getCacheLineSize(int cpu_index = 0);

/**
 * Filesystem statistics information.
 *
 * @author zer0
 * @date   2018-01-08
 */
struct FilesystemStatistics
{
    std::size_t  bsize;     ///< Filesystem block size.
    std::size_t  frsize;    ///< Fragment size.
    std::size_t  blocks;    ///< Size of fs in @c frsize units.
    std::size_t  bfree;     ///< Number of free blocks.
    std::size_t  bavail;    ///< Number of free blocks for unprivileged users.
    std::size_t  files;     ///< Number of inodes.
    std::size_t  ffree;     ///< Number of free inodes.
    std::size_t  favail;    ///< Number of free inodes for unprivileged users.
    std::size_t  fsid;      ///< Filesystem ID.
    std::size_t  flag;      ///< Mount flags.
    std::size_t  namemax;   ///< Maximum filename length.

    std::size_t total_byte;
    std::size_t  free_byte;
    std::size_t  used_byte;

    FilesystemStatistics() { clear(); }
    ~FilesystemStatistics() { /* EMPTY. */ }

    inline void clear() TBAG_NOEXCEPT
    {
        bsize      = 0;
        frsize     = 0;
        blocks     = 0;
        bfree      = 0;
        bavail     = 0;
        files      = 0;
        ffree      = 0;
        favail     = 0;
        fsid       = 0;
        flag       = 0;
        namemax    = 0;
        total_byte = 0;
        free_byte  = 0;
        used_byte  = 0;
    }
};

TBAG_API bool getFilesystemInfo(std::string const & path, FilesystemStatistics & result);

/** Gets the resident set size (RSS) for the current process. */
TBAG_API std::size_t getResidentSetMemory();

/** Gets the current system uptime. */
TBAG_API double getUptime();

struct CpuTimes
{
    uint64_t user;
    uint64_t nice;
    uint64_t sys;
    uint64_t idle;
    uint64_t irq;
};

/** Data type for CPU information. */
struct CpuInfo
{
    std::string model;
    int speed;
    CpuTimes times;
};

TBAG_API std::vector<CpuInfo> getCpuInfos();

TBAG_CONSTEXPR std::size_t const PHYSICAL_ADDRESS_SIZE = 6;

using PhysicalAddress = std::array<char, PHYSICAL_ADDRESS_SIZE>;

union SocketAddress
{
    struct sockaddr  common;
    struct sockaddr_in  in4;
    struct sockaddr_in6 in6;
};

/** Data type for interface addresses. */
struct InterfaceAddress
{
    std::string name;
    PhysicalAddress physical_address;
    bool is_internal;

    SocketAddress address;
    SocketAddress netmask;
};

TBAG_API std::vector<InterfaceAddress> getInterfaceAddresses();
TBAG_API std::string convertPhysicalToString(PhysicalAddress const & physical);

/** Changes the current working directory. */
TBAG_API void changeDirectory(std::string const & dir);

/**
 * Gets the load average.
 *
 * @see <http://en.wikipedia.org/wiki/Load_(computing)>
 *
 * @warning
 *  Returns [0,0,0] on Windows (i.e., it's not implemented).
 */
TBAG_API std::vector<double> getLoadAverage();

/** Data type for password file information. */
struct Password
{
    std::string username;
    std::string shell;
    std::string homedir;

    long uid;
    long gid;
};

/** Gets a subset of the password file entry for the current effective uid (not the real uid). */
TBAG_API Password getPassword();

/** Gets memory information (in bytes). */
TBAG_API uint64_t getTotalMemory();

/** Returns the current high-resolution real time. This is expressed in nanoseconds. */
TBAG_API uint64_t getHighResolutionTime();

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__

