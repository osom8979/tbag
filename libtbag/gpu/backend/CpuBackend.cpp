/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdlib>

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
    checkType(platform.type);
    GpuPlatformInfo info(platform);
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int CpuBackend::getDeviceCount(GpuPlatform const & platform) const
{
    checkType(platform.type);
    return 1;
}

GpuDevices CpuBackend::getDeviceList(GpuPlatform const & platform) const
{
    checkType(platform.type);
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo CpuBackend::getDeviceInfo(GpuDevice const & device) const
{
    checkType(device.type);
    GpuDeviceInfo info(device);
    info.name = "CPU";
    return info;
}

GpuContext CpuBackend::createContext(GpuDevice const & device) const
{
    checkType(device.type);
    return GpuContext(device, 0);
}

bool CpuBackend::releaseContext(GpuContext & context) const
{
    checkType(context.type);
    context.context_id = UNKNOWN_GPU_ID;
    return true;
}

GpuMemory CpuBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory memory(context);
    memory.data = ::malloc(size);
    memory.size = size;
    return memory;
}

bool CpuBackend::free(GpuMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("CpuBackend::free() Illegal memory.");
        return false;
    }
    ::free(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

