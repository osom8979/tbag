/**
 * @file   GpuBackend.cpp
 * @brief  GpuBackend class implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/GpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/gpu/CpuBackend.hpp>
#include <libtbag/gpu/CudaBackend.hpp>
#include <libtbag/gpu/OpenCLBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

GpuBackendInterface * createGpu(BackendType type)
{
    if (type == BackendType::BT_UNKNOWN) {
        if (isCudaSupport()) {
            type = BackendType::BT_CUDA;
        } else if (isOpenCLSupport()) {
            type = BackendType::BT_OPENCL;
        } else {
            type = BackendType::BT_CPU;
        }
    }

    switch (type) {
    case BackendType::BT_CPU:    return new CpuBackend();
    case BackendType::BT_CUDA:   return new CudaBackend();
    case BackendType::BT_OPENCL: return new OpenCLBackend();
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return nullptr;
}

std::shared_ptr<GpuBackendInterface> createSharedGpu(BackendType type)
{
    return std::shared_ptr<GpuBackendInterface>(createGpu(type));
}

std::unique_ptr<GpuBackendInterface> createUniqueGpu(BackendType type)
{
    return std::unique_ptr<GpuBackendInterface>(createGpu(type));
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

