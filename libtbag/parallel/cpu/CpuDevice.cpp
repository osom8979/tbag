/**
 * @file   CpuDevice.cpp
 * @brief  CpuDevice class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/parallel/cpu/CpuDevice.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cpu      {

/**
 * Parallel Computing - CPU Context.
 *
 * @author zer0
 * @date   2019-02-22
 */
struct _ParallelCpuContext
{
    _ParallelCpuContext()
    {
        // EMPTY.
    }

    ~_ParallelCpuContext()
    {
        // EMPTY.
    }
};

// -------
// Methods
// -------

bool isSupport() TBAG_NOEXCEPT
{
    return true;
}

int getPlatformCount()
{
    return 1;
}

ParallelPlatformIds getPlatformList()
{
    return {0};
}

ParallelPlatformInfo getPlatformInfo(ParallelPlatformId platform)
{
    ParallelPlatformInfo info;
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = libtbag::util::getTbagVersion().toLongString();
    return info;
}

int getDeviceCount(ParallelPlatformId platform)
{
    return 1;
}

ParallelDeviceIds getDeviceList(ParallelPlatformId platform)
{
    return {0};
}

ParallelDeviceInfo getDeviceInfo(ParallelDeviceId device)
{
    ParallelDeviceInfo info;
    info.name = "CPU";
    return info;
}

ParallelContextId createContext(ParallelDeviceId device)
{
    auto * context = new (std::nothrow) _ParallelCpuContext();
    if (context == nullptr) {
        return UNKNOWN_ID;
    }
    return (ParallelContextId)context;
}

bool deleteContext(ParallelContextId context)
{
    delete ((_ParallelCpuContext*)context);
    return true;
}

} // namespace cpu
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

