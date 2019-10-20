/**
 * @file   ResourceUsage.hpp
 * @brief  ResourceUsage class prototype.
 * @author zer0
 * @date   2019-10-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_RESOURCEUSAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_RESOURCEUSAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ErrPair.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

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

using ErrResourceUsage = ErrPair<ResourceUsage>;

/**
 * Gets the resource usage measures for the current process.
 *
 * @warning
 * On Windows not all fields are set, the unsupported fields are filled with zeroes.
*/
TBAG_API ErrResourceUsage getResourceUsage();
TBAG_API ErrResourceUsage getResourceUsage(int pid);

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_RESOURCEUSAGE_HPP__

