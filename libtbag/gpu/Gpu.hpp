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

#include <libtbag/gpu/GpuInterface.hpp>
#include <libtbag/gpu/accel/AccelBackend.hpp>
#include <libtbag/gpu/cpu/CpuBackend.hpp>
#include <libtbag/gpu/cuda/CudaBackend.hpp>
#include <libtbag/gpu/opencl/OpenCLBackend.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

TBAG_API int             getPlatformCount(GpuType type);
TBAG_API GpuPlatforms    getPlatformList (GpuType type);
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

//template <typename Predicated>
//void runIfSupported(std::vector<GpuBackendType> const & types, Predicated predicated)
//{
//    for (auto & type : types) {
//        auto gpu = createGpuContext(type);
//        if (gpu && gpu->isSupport()) {
//            predicated(gpu);
//        }
//    }
//}

//template <typename Predicated>
//void runAllIfSupported(Predicated predicated)
//{
//    runIfSupported({GpuBackendType::GBT_CPU,
//                    GpuBackendType::GBT_ACCEL,
//                    GpuBackendType::GBT_CUDA,
//                    GpuBackendType::GBT_OPENCL}, predicated);
//}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

