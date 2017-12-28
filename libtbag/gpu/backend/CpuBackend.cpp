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

int CpuBackend::getPlatformCount() const
{
    return 1;
}

CpuBackend::Platforms CpuBackend::getPlatformList() const
{
    GpuPlatform plat(getType(), 0);
    plat.name = "CPU";
    plat.vendor = "TBAG";
    plat.version = util::getTbagVersion().toLongString();
    return {plat};
}

int CpuBackend::getDeviceCount() const
{
    return 1;
}

CpuBackend::Devices CpuBackend::getDeviceList() const
{
    return {GpuDevice(getType(), 0)};
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

