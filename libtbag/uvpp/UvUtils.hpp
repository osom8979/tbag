/**
 * @file   UvUtils.hpp
 * @brief  uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <array>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

/** Store the program arguments. Required for getting/setting the process title. */
TBAG_API char ** setupArgs(int argc, char ** argv);

/** Gets the title of the current process. */
TBAG_API std::string getProcessTitle();

/** Sets the current process title. */
TBAG_API void setProcessTitle(std::string const & title);

/** Gets the resident set size (RSS) for the current process. */
TBAG_API std::size_t getResidentSetMemory();

/** Gets the current system uptime. */
TBAG_API double getUptime();

struct TimeVal
{
    long sec;
    long usec;
};

/** Data type for resource usage results. */
struct ResourceUsage
{
    TimeVal  utime;     ///< user CPU time used
    TimeVal  stime;     ///< system CPU time used
    uint64_t maxrss;    ///< maximum resident set size
    uint64_t ixrss;     ///< integral shared memory size (X)
    uint64_t idrss;     ///< integral unshared data size (X)
    uint64_t isrss;     ///< integral unshared stack size (X)
    uint64_t minflt;    ///< page reclaims (soft page faults) (X)
    uint64_t majflt;    ///< page faults (hard page faults)
    uint64_t nswap;     ///< swaps (X)
    uint64_t inblock;   ///< block input operations
    uint64_t oublock;   ///< block output operations
    uint64_t msgsnd;    ///< IPC messages sent (X)
    uint64_t msgrcv;    ///< IPC messages received (X)
    uint64_t nsignals;  ///< signals received (X)
    uint64_t nvcsw;     ///< voluntary context switches (X)
    uint64_t nivcsw;    ///< involuntary context switches (X)
};

/**
 * Gets the resource usage measures for the current process.
 *
 * @warning
 * On Windows not all fields are set, the unsupported fields are filled with zeroes.
*/
TBAG_API ResourceUsage getResourceUsage();

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

/**
 * Retrieves the environment variable specified by name.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err getEnv(std::string const & name, std::string & value);

/**
 * Creates or updates the environment variable specified by name with value.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err setEnv(std::string const & name, std::string const & value);

/**
 * Deletes the environment variable specified by name.
 * If no such environment variable exists, this function returns successfully.
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API Err unsetEnv(std::string const & name);

/**
 * Returns the hostname as a null-terminated string
 *
 * @warning
 *  This function is not thread safe.
 */
TBAG_API std::string getHostName();

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UVUTILS_HPP__

