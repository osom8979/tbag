/**
 * @file   AccelBackend.cpp
 * @brief  AccelBackend class implementation.
 * @author zer0
 * @date   2018-01-13
 */

#include <libtbag/gpu/accel/AccelBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu   {
namespace accel {

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
    return {GpuPlatform(GpuType::GT_ACCEL, 0)};
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    GpuPlatformInfo info;
    info.name    = "ACCEL";
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
    info.name = "ACCEL";
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkCpuGpuType(device);
    return SharedGpuContext(new AccelContext(device, 0));
}

// ----------------------------
// AccelContext implementation.
// ----------------------------

AccelContext::AccelContext(GpuDevice const & d, GpuId c) : cpu::CpuContext(d, c)
{
    // EMPTY.
}

AccelContext::~AccelContext()
{
    // EMPTY.
}

// @formatter:off
bool AccelContext::isSupport() const TBAG_NOEXCEPT { return  true; }
bool AccelContext::isHost   () const TBAG_NOEXCEPT { return  true; }
bool AccelContext::isDevice () const TBAG_NOEXCEPT { return false; }
bool AccelContext::isStream () const TBAG_NOEXCEPT { return  true; }
// @formatter:on

} // namespace accel
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

