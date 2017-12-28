/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

#if defined(USE_CUDA)
#include <cuda.h>
#include <cuda_runtime.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType CudaBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_CUDA;
}

bool CudaBackend::isSupport() const TBAG_NOEXCEPT
{
    return isCudaSupport();
}

int CudaBackend::getPlatformCount() const
{
    return 1;
}

CudaBackend::Platforms CudaBackend::getPlatformList() const
{
    GpuPlatform plat(getType(), 0);
    plat.name = "CUDA";
    plat.vendor = "NVIDIA";
#if defined(USE_CUDA)
# if defined(CUDA_VERSION)
    plat.version += string::fformat("API({})", TO_STRING(CUDA_VERSION));
# endif

    int driver_version = 0;
    cudaError_t driver_code = ::cudaDriverGetVersion(&driver_version);
    if (driver_code == cudaSuccess) {
        plat.version += string::fformat("DRIVER({})", driver_version);
    } else {
        tDLogE("CudaBackend::getPlatformList() CUDA driver version error: {}", ::cudaGetErrorString(driver_code));
    }

    int runtime_version = 0;
    cudaError_t runtime_code = ::cudaRuntimeGetVersion(&runtime_version);
    if (runtime_code == cudaSuccess) {
        plat.version += string::fformat("RUNTIME({})", runtime_version);
    } else {
        tDLogE("CudaBackend::getPlatformList() CUDA runtime version error: {}", ::cudaGetErrorString(runtime_code));
    }
#endif
    return {plat};
}

int CudaBackend::getDeviceCount() const
{
    int result = 0;
#if defined(USE_CUDA)
    cudaError_t code = ::cudaGetDeviceCount(&result);
    if (code != cudaSuccess) {
        tDLogE("CudaBackend::getDeviceCount() CUDA error: {}", ::cudaGetErrorString(code));
        return 0;
    }
#endif
    return result;
}

CudaBackend::Devices CudaBackend::getDeviceList() const
{
    Devices result;
    for (int i = 0; i < getDeviceCount(); ++i) {
        GpuDevice device(getType(), i);
#if defined(USE_CUDA)
        cudaDeviceProp prop;
        cudaError_t code = ::cudaGetDeviceProperties(&prop, i);
        if (code == cudaSuccess) {
        } else {
            tDLogE("CudaBackend::getDeviceList() CUDA error: {}", ::cudaGetErrorString(code));
        }
#endif
        result.push_back(device);
    }
    return result;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

