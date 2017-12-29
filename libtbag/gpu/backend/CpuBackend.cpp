/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType CpuBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_CPU;
}

bool CpuBackend::isSupport() const TBAG_NOEXCEPT
{
    return true;
}

bool CpuBackend::isHost() const TBAG_NOEXCEPT
{
    return true;
}

bool CpuBackend::isDevice() const TBAG_NOEXCEPT
{
    return false;
}

int CpuBackend::getPlatformCount() const
{
    return 1;
}

GpuPlatforms CpuBackend::getPlatformList() const
{
    return {GpuPlatform(getType(), 0)};
}

GpuPlatformInfo CpuBackend::getPlatformInfo(GpuPlatform const & platform) const
{
    GpuPlatformInfo info(platform);
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int CpuBackend::getDeviceCount(GpuPlatform const & platform) const
{
    return 1;
}

GpuDevices CpuBackend::getDeviceList(GpuPlatform const & platform) const
{
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo CpuBackend::getDeviceInfo(GpuDevice const & device) const
{
    GpuDeviceInfo info(device);
    info.name = "CPU";
    return info;
}

GpuContext CpuBackend::createContext(GpuDevice const & device) const
{
    return GpuContext(device);
}

bool CpuBackend::releaseContext(GpuContext & context) const
{
    context.context_id = UNKNOWN_GPU_ID;
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

