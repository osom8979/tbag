/**
 * @file   CudaDevice.cpp
 * @brief  CudaDevice class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/parallel/cuda/CudaDevice.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

#if defined(USE_CUDA)
# include <cuda.h>
# include <cuda_runtime.h>
#else
# include <libtbag/dummy/Cuda.hpp>
using namespace libtbag::dummy::cuda;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cuda     {

/**
 * Parallel Computing - CUDA Context.
 *
 * @author zer0
 * @date   2019-02-22
 */
struct _PcCudaContext
{
    _PcCudaContext()
    {
        // EMPTY.
    }

    ~_PcCudaContext()
    {
        // EMPTY.
    }
};

// -------
// Methods
// -------

bool isSupport() TBAG_NOEXCEPT
{
#if defined(USE_CUDA)
    return true;
#else
    return false;
#endif
}

int getPlatformCount()
{
    return 1;
}

PcPlatformIds getPlatformList()
{
    return {0};
}

PcPlatformInfo getPlatformInfo(PcPlatformId platform)
{
    PcPlatformInfo info;
    info.name   = "CUDA";
    info.vendor = "NVIDIA";
    int driver_version = 0;
    cudaError_t driver_code = cudaDriverGetVersion(&driver_version);
    if (driver_code == cudaSuccess) {
        info.version += string::fformat("DRIVER({})", driver_version);
    } else {
        tDLogE("getPlatformInfo() CUDA cudaDriverGetVersion() error: {}", cudaGetErrorString(driver_code));
    }

    int runtime_version = 0;
    cudaError_t runtime_code = cudaRuntimeGetVersion(&runtime_version);
    if (runtime_code == cudaSuccess) {
        info.version += string::fformat("RUNTIME({})", runtime_version);
    } else {
        tDLogE("getPlatformInfo() CUDA cudaRuntimeGetVersion() error: {}", cudaGetErrorString(runtime_code));
    }

#if defined(CUDA_VERSION)
    info.version += libtbag::string::fformat("API({})", TO_STRING(CUDA_VERSION));
#endif
    return info;
}

int getDeviceCount(PcPlatformId platform)
{
    int result = 0;
    cudaError_t code = cudaGetDeviceCount(&result);
    if (code != cudaSuccess) {
        tDLogE("getDeviceCount() CUDA cudaGetDeviceCount() error: {}", cudaGetErrorString(code));
        return 0;
    }
    return result;
}

PcDeviceIds getDeviceList(PcPlatformId platform)
{
    PcDeviceIds result;
    for (int i = 0; i < getDeviceCount(platform); ++i) {
        result.emplace_back(i);
    }
    return result;
}

PcDeviceInfo getDeviceInfo(PcDeviceId device)
{
    PcDeviceInfo info;
    cudaDeviceProp prop;
    cudaError_t code = cudaGetDeviceProperties(&prop, device);
    if (code != cudaSuccess) {
        tDLogE("getDeviceInfo() CUDA cudaGetDeviceProperties() error: {}", cudaGetErrorString(code));
        return info;
    }

    info.name           = prop.name;
    info.device_version = string::fformat("{}.{}", prop.major, prop.minor);
    info.global_memory  = prop.totalGlobalMem;

    int driver_version = 0;
    cudaError_t driver_code = cudaDriverGetVersion(&driver_version);
    if (driver_code == cudaSuccess) {
        info.driver_version = std::to_string(driver_version);
    } else {
        tDLogE("getDeviceInfo() CUDA cudaDriverGetVersion() error: {}", cudaGetErrorString(driver_code));
    }

    info.insert(TBAG_GPU_DEVICE_INFO_SHARED_MEM_PER_BLOCK       , prop.sharedMemPerBlock);
    info.insert(TBAG_GPU_DEVICE_INFO_REGS_PER_BLOCK             , prop.regsPerBlock);
    info.insert(TBAG_GPU_DEVICE_INFO_WARP_SIZE                  , prop.warpSize);
    info.insert(TBAG_GPU_DEVICE_INFO_MEM_PITCH                  , prop.memPitch);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_THREADS_PER_BLOCK      , prop.maxThreadsPerBlock);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_0          , prop.maxThreadsDim[0]);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_1          , prop.maxThreadsDim[1]);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_2          , prop.maxThreadsDim[2]);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_0            , prop.maxGridSize[0]);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_1            , prop.maxGridSize[1]);
    info.insert(TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_2            , prop.maxGridSize[2]);
    info.insert(TBAG_GPU_DEVICE_INFO_CLOCK_RATE                 , prop.clockRate);
    info.insert(TBAG_GPU_DEVICE_INFO_TOTAL_CONST_MEM            , prop.totalConstMem);
    info.insert(TBAG_GPU_DEVICE_INFO_TEXTURE_ALIGNMENT          , prop.textureAlignment);
    info.insert(TBAG_GPU_DEVICE_INFO_DEVICE_OVERLAP             , prop.deviceOverlap);
    info.insert(TBAG_GPU_DEVICE_INFO_MULTI_PROCESSOR_COUNT      , prop.multiProcessorCount);
    info.insert(TBAG_GPU_DEVICE_INFO_KERNEL_EXEC_TIMEOUT_ENABLED, prop.kernelExecTimeoutEnabled);
    info.insert(TBAG_GPU_DEVICE_INFO_INTEGRATED                 , prop.integrated);
    info.insert(TBAG_GPU_DEVICE_INFO_CAN_MAP_HOST_MEMORY        , prop.canMapHostMemory);
    if (prop.computeMode == cudaComputeModeExclusive) {
        info.insert(TBAG_GPU_DEVICE_INFO_COMPUTE_MODE, TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_EXCLUSIVE);
    } else if (prop.computeMode == cudaComputeModeProhibited) {
        info.insert(TBAG_GPU_DEVICE_INFO_COMPUTE_MODE, TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_PROHIBITED);
    } else {
        assert(prop.computeMode == cudaComputeModeDefault);
        info.insert(TBAG_GPU_DEVICE_INFO_COMPUTE_MODE, TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_DEFAULT);
    }
    info.insert(TBAG_GPU_DEVICE_INFO_CONCURRENT_KERNELS         , prop.concurrentKernels);
    info.insert(TBAG_GPU_DEVICE_INFO_ECC_ENABLED                , prop.ECCEnabled);
    info.insert(TBAG_GPU_DEVICE_INFO_PCI_BUS_ID                 , prop.pciBusID);
    info.insert(TBAG_GPU_DEVICE_INFO_PCI_DEVICE_ID              , prop.pciDeviceID);
    info.insert(TBAG_GPU_DEVICE_INFO_TCC_DRIVER                 , prop.tccDriver);
    return info;
}

PcContextId createContext(PcDeviceId device)
{
    auto * context = new (std::nothrow) _PcCudaContext();
    if (context == nullptr) {
        return UNKNOWN_ID;
    }
    return (PcContextId)context;
}

bool deleteContext(PcContextId context)
{
    delete ((_PcCudaContext*)context);
    return true;
}

} // namespace cuda
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

