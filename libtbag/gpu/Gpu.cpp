/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/gpu/backend/AccelBackend.hpp>
#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/gpu/backend/CudaBackend.hpp>
#include <libtbag/gpu/backend/OpenCLBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

UniqueGpu createGpuContext(GpuBackendType type)
{
    switch (type) {
    case GpuBackendType::GBT_CPU:      return    createCpuContext();
    case GpuBackendType::GBT_ACCEL:    return  createAccelContext();
    case GpuBackendType::GBT_CUDA:     return   createCudaContext();
    case GpuBackendType::GBT_OPENCL:   return createOpenCLContext();
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT();  break;
    }
    return UniqueGpu();
}

UniqueGpu createCpuContext()
{
    return UniqueGpu(new backend::CpuBackend());
}

UniqueGpu createAccelContext()
{
    return UniqueGpu(new backend::AccelBackend());
}

UniqueGpu createCudaContext()
{
    return UniqueGpu(new backend::CudaBackend());
}

UniqueGpu createOpenCLContext()
{
    return UniqueGpu(new backend::OpenCLBackend());
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

