/**
 * @file   Gpu.hpp
 * @brief  Gpu class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/gpu/backend/GpuBackend.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

using GpuBackendType = backend::GpuBackendType;
using GpuDevice      = backend::GpuDevice;
using GpuBackend     = backend::GpuBackend;
using UniqueGpu      = std::unique_ptr<GpuBackend>;

TBAG_API UniqueGpu createGpuContext(GpuBackendType type);

TBAG_API UniqueGpu    createCpuContext();
TBAG_API UniqueGpu  createAccelContext();
TBAG_API UniqueGpu   createCudaContext();
TBAG_API UniqueGpu createOpenCLContext();

template <typename Predicated>
void runIfSupported(std::vector<GpuBackendType> const & types, Predicated predicated)
{
    for (auto & type : types) {
        auto gpu = createGpuContext(type);
        if (gpu && gpu->isSupport()) {
            predicated(gpu);
        }
    }
}

template <typename Predicated>
void runAllIfSupported(Predicated predicated)
{
    runIfSupported({GpuBackendType::GBT_CPU,
                    GpuBackendType::GBT_ACCEL,
                    GpuBackendType::GBT_CUDA,
                    GpuBackendType::GBT_OPENCL}, predicated);
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

