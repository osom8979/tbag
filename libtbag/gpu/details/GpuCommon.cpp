/**
 * @file   GpuCommon.cpp
 * @brief  GpuCommon class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

char const * getGpuTypeString(GpuType type) TBAG_NOEXCEPT
{
    switch (type) {
    case GpuType::GT_CPU:     return "CPU";
    case GpuType::GT_ACCEL:   return "ACCEL";
    case GpuType::GT_CUDA:    return "CUDA";
    case GpuType::GT_OPENCL:  return "OPENCL";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;
    }
    return "UNKNOWN";
}

char const * getHostMemoryTypeString(HostMemoryFlag flag) TBAG_NOEXCEPT
{
    switch (flag) {
    case HostMemoryFlag::HMF_DEFAULT: return "DEFAULT";
    case HostMemoryFlag::HMF_PINNED:  return "PINNED";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;
    }
    return "UNKNOWN";
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

