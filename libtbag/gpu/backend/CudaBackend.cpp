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

bool CudaBackend::isHost() const TBAG_NOEXCEPT
{
    return false;
}

bool CudaBackend::isDevice() const TBAG_NOEXCEPT
{
    return true;
}

int CudaBackend::getPlatformCount() const
{
    return 1;
}

GpuPlatforms CudaBackend::getPlatformList() const
{
    return {GpuPlatform(getType(), 0)};
}

GpuPlatformInfo CudaBackend::getPlatformInfo(GpuPlatform const & platform) const
{
    GpuPlatformInfo info(platform);
    info.name   = "CUDA";
    info.vendor = "NVIDIA";
#if defined(USE_CUDA)
    int driver_version = 0;
    cudaError_t driver_code = ::cudaDriverGetVersion(&driver_version);
    if (driver_code == cudaSuccess) {
        info.version += string::fformat("DRIVER({})", driver_version);
    } else {
        tDLogE("CudaBackend::getPlatformInfo() CUDA driver version error: {}", ::cudaGetErrorString(driver_code));
    }

    int runtime_version = 0;
    cudaError_t runtime_code = ::cudaRuntimeGetVersion(&runtime_version);
    if (runtime_code == cudaSuccess) {
        info.version += string::fformat("RUNTIME({})", runtime_version);
    } else {
        tDLogE("CudaBackend::getPlatformInfo() CUDA runtime version error: {}", ::cudaGetErrorString(runtime_code));
    }

# if defined(CUDA_VERSION)
    info.version += string::fformat("API({})", TO_STRING(CUDA_VERSION));
# endif
#endif
    return info;
}

int CudaBackend::getDeviceCount(GpuPlatform const & platform) const
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

GpuDevices CudaBackend::getDeviceList(GpuPlatform const & platform) const
{
    GpuDevices result;
    for (int i = 0; i < getDeviceCount(platform); ++i) {
        result.emplace_back(GpuDevice(platform, i));
    }
    return result;
}

GpuDeviceInfo CudaBackend::getDeviceInfo(GpuDevice const & device) const
{
    GpuDeviceInfo info(device);
#if defined(USE_CUDA)
    cudaDeviceProp prop;
    cudaError_t code = ::cudaGetDeviceProperties(&prop, device.device_number);
    if (code == cudaSuccess) {
        info.name = prop.name;
    } else {
        tDLogE("CudaBackend::getDeviceInfo() CUDA error: {}", ::cudaGetErrorString(code));
    }
#endif
    return info;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

