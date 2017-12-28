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

char const * GpuBackend::getTypeString() const TBAG_NOEXCEPT
{
    return getGpuBackendString(getType());
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

