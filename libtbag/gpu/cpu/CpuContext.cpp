/**
 * @file   CpuContext.cpp
 * @brief  CpuContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/cpu/CpuContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

bool isSupport() TBAG_NOEXCEPT
{
    return true;
}

int getPlatformCount()
{
    return 1;
}

GpuPlatforms getPlatformList()
{
    return {GpuPlatform(GpuType::GT_CPU, 0)};
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    GpuPlatformInfo info;
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    return 1;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkCpuGpuType(device);
    GpuDeviceInfo info;
    info.name = "CPU";
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkCpuGpuType(device);
    return SharedGpuContext(new CpuContext(device, 0));
}

// --------------------------
// CpuContext implementation.
// --------------------------

CpuContext::CpuContext(GpuDevice const & d, GpuId c) : GpuContext(d, c)
{
    // EMPTY.
}

CpuContext::~CpuContext()
{
    // EMPTY.
}

// @formatter:off
bool CpuContext::isSupport() const TBAG_NOEXCEPT { return  true; }
bool CpuContext::isHost   () const TBAG_NOEXCEPT { return  true; }
bool CpuContext::isDevice () const TBAG_NOEXCEPT { return false; }
bool CpuContext::isStream () const TBAG_NOEXCEPT { return  true; }
// @formatter:on

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

