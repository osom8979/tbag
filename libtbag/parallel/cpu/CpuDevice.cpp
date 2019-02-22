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
struct _PcCpuContext
{
    _PcCpuContext()
    {
        // EMPTY.
    }

    ~_PcCpuContext()
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

PcPlatformIds getPlatformList()
{
    return {0};
}

PcPlatformInfo getPlatformInfo(PcPlatformId platform)
{
    PcPlatformInfo info;
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = libtbag::util::getTbagVersion().toLongString();
    return info;
}

int getDeviceCount(PcPlatformId platform)
{
    return 1;
}

PcDeviceIds getDeviceList(PcPlatformId platform)
{
    return {0};
}

PcDeviceInfo getDeviceInfo(PcDeviceId device)
{
    PcDeviceInfo info;
    info.name = "CPU";
    return info;
}

PcContextId createContext(PcDeviceId device)
{
    auto * context = new (std::nothrow) _PcCpuContext();
    if (context == nullptr) {
        return UNKNOWN_ID;
    }
    return (PcContextId)context;
}

bool deleteContext(PcContextId context)
{
    delete ((_PcCpuContext*)context);
    return true;
}

} // namespace cpu
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

