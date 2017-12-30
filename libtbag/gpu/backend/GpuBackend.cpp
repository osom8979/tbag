/**
 * @file   GpuBackend.cpp
 * @brief  GpuBackend class implementation.
 * @author zer0
 * @date   2017-12-27
 */

#include <libtbag/gpu/backend/GpuBackend.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

char const * getGpuBackendString(GpuBackendType type) TBAG_NOEXCEPT
{
    switch (type) {
    case GpuBackendType::GBT_CPU:     return "CPU";
    case GpuBackendType::GBT_ACCEL:   return "ACCEL";
    case GpuBackendType::GBT_CUDA:    return "CUDA";
    case GpuBackendType::GBT_OPENCL:  return "OPENCL";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;
    }
    return "UNKNOWN";
}

// --------------------------
// GpuBackend implementation.
// --------------------------

void GpuBackend::checkType(GpuBackendType type) const throw(GpuTypeMismatchException)
{
    if (type != getType()) {
        throw GpuTypeMismatchException();
    }
}

char const * GpuBackend::getTypeString() const TBAG_NOEXCEPT
{
    return getGpuBackendString(getType());
}

GpuContext GpuBackend::createContext(int platform_index, int device_index) const
{
    auto const PLATFORMS = getPlatformList();
    if (platform_index < 0 || PLATFORMS.size() <= platform_index) {
        return GpuContext();
    }
    auto const DEVICES = getDeviceList(PLATFORMS[platform_index]);
    if (device_index < 0 || DEVICES.size() <= device_index) {
        return GpuContext();
    }
    return createContext(DEVICES[device_index]);
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

