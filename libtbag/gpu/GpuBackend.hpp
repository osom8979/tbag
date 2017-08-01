/**
 * @file   GpuBackend.hpp
 * @brief  GpuBackend class prototype.
 * @author zer0
 * @date   2017-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

enum class BackendType
{
    BT_UNKNOWN, // Auto select.
    BT_CPU,
    BT_CUDA,
    BT_OPENCL,
};

TBAG_CONSTEXPR bool isCudaSupport() TBAG_NOEXCEPT
{
#if defined(USE_CUDA)
    return true;
#else
    return false;
#endif
}

TBAG_CONSTEXPR bool isOpenCLSupport() TBAG_NOEXCEPT
{
#if defined(USE_OPENCL)
    return true;
#else
    return false;
#endif
}

struct GpuBackendInterface
{
    GpuBackendInterface() { /* EMPTY. */ }
    virtual ~GpuBackendInterface() { /* EMPTY. */ }

    virtual BackendType getType() const TBAG_NOEXCEPT = 0;
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual int getDeviceCount() const = 0;
};

TBAG_API GpuBackendInterface * createGpu(BackendType type = BackendType::BT_UNKNOWN);
TBAG_API std::shared_ptr<GpuBackendInterface> createSharedGpu(BackendType type = BackendType::BT_UNKNOWN);
TBAG_API std::unique_ptr<GpuBackendInterface> createUniqueGpu(BackendType type = BackendType::BT_UNKNOWN);

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__

