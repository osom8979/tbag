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
    checkType(platform.type);
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
    checkType(platform.type);
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
    checkType(platform.type);
    GpuDevices result;
    for (int i = 0; i < getDeviceCount(platform); ++i) {
        result.emplace_back(GpuDevice(platform, i));
    }
    return result;
}

GpuDeviceInfo CudaBackend::getDeviceInfo(GpuDevice const & device) const
{
    checkType(device.type);
    GpuDeviceInfo info(device);
#if defined(USE_CUDA)
    cudaDeviceProp prop;
    cudaError_t code = ::cudaGetDeviceProperties(&prop, device.device_id);
    if (code == cudaSuccess) {
        info.name = prop.name;
    } else {
        tDLogE("CudaBackend::getDeviceInfo() CUDA error: {}", ::cudaGetErrorString(code));
    }
#endif
    return info;
}

GpuContext CudaBackend::createContext(GpuDevice const & device) const
{
    checkType(device.type);
    return GpuContext(device, 0);
}

bool CudaBackend::releaseContext(GpuContext & context) const
{
    checkType(context.type);
    context.context_id = UNKNOWN_GPU_ID;
    return true;
}

GpuQueue CudaBackend::createQueue(GpuContext const & context) const
{
    checkType(context.type);
    GpuQueue result(context);
#if defined(USE_CUDA)
    cudaStream_t stream;
    cudaError_t code = ::cudaStreamCreate(&stream);
    if (code == cudaSuccess) {
        result.queue_id = (GpuId)stream;
    } else {
        tDLogE("CudaBackend::createQueue() CUDA error: {}", ::cudaGetErrorString(code));
    }
#endif
    return result;
}

bool CudaBackend::releaseQueue(GpuQueue & queue) const
{
    checkType(queue.type);
    if (queue.isUnknownQueue()) {
        tDLogE("CudaBackend::releaseQueue() Illegal queue.");
        return false;
    }
#if defined(USE_CUDA)
    cudaError_t code = ::cudaStreamDestroy((cudaStream_t)queue.queue_id);
    queue.queue_id = UNKNOWN_GPU_ID;
    if (code != cudaSuccess) {
        tDLogE("CudaBackend::releaseQueue() CUDA error: {}", ::cudaGetErrorString(code));
        return false;
    }
#endif
    return true;
}

GpuMemory CudaBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory memory(context);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMalloc((void**)&memory.data, size);
    if (code == cudaSuccess) {
        memory.size = size;
    } else {
        tDLogE("CudaBackend::malloc() CUDA error: {}", ::cudaGetErrorString(code));
    }
#endif
    return memory;
}

bool CudaBackend::free(GpuMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("CudaBackend::free() Illegal memory.");
        return false;
    }
#if defined(USE_CUDA)
    cudaError_t code = ::cudaFree(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    if (code != cudaSuccess) {
        tDLogE("CudaBackend::free() CUDA error: {}", ::cudaGetErrorString(code));
        return false;
    }
#endif
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

