/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2018-01-13
 */

#include <libtbag/gpu/cuda/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

#if defined(USE_CUDA)
#include <cuda.h>
#include <cuda_runtime.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu  {
namespace cuda {

/** ASCII string identifying the device */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_NAME = "name";
/** Version string */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_VERSION = "version";

/** is the maximum amount of shared memory available to a thread block in bytes; @n
 * this amount is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_SHARED_MEM_PER_BLOCK = "sharedMemPerBlock";
/** is the maximum number of 32-bit registers available to a thread block; @n
 * this number is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_REGS_PER_BLOCK = "regsPerBlock";
/** is the warp size in threads; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_WARP_SIZE = "warpSize";
/** is the maximum pitch in bytes allowed by the memory copy functions @n
 * that involve memory regions allocated through cudaMallocPitch(); */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MEM_PITCH = "memPitch";
/** is the maximum number of threads per block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_PER_BLOCK = "maxThreadsPerBlock";
/** contains the maximum size of each dimension of a block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_0 = "maxThreadsDim0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_1 = "maxThreadsDim1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_2 = "maxThreadsDim2";
/** contains the maximum size of each dimension of a grid; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_0 = "maxGridSize0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_1 = "maxGridSize1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_2 = "maxGridSize2";
/** is the clock frequency in kilohertz; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CLOCK_RATE = "clockRate";
/** is the total amount of constant memory available on the device in bytes; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TOTAL_CONST_MEM = "totalConstMem";
/** is the alignment requirement; texture base addresses that are aligned @n
 * to textureAlignment bytes do not need an offset applied to texture fetches; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TEXTURE_ALIGNMENT = "textureAlignment";
/** is 1 if the device can concurrently copy memory between host and device while executing a kernel, or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_DEVICE_OVERLAP = "deviceOverlap";
/** is the number of multiprocessors on the device; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MULTI_PROCESSOR_COUNT = "multiProcessorCount";
/** is 1 if there is a run time limit for kernels executed on the device, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_KERNEL_EXEC_TIMEOUT_ENABLED = "kernelExecTimeoutEnabled";
/** is 1 if the device is an integrated (motherboard) GPU and 0 if it is a discrete (card) component. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_INTEGRATED = "integrated";
/** is 1 if the device can map host memory into the CUDA address space @n
 * for use with cudaHostAlloc()/cudaHostGetDevicePointer(), or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CAN_MAP_HOST_MEMORY = "canMapHostMemory";

/** is the compute mode that the device is currently in. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE = "computeMode";
/** Default mode - Device is not restricted and multiple threads can use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_DEFAULT = "cudaComputeModeDefault";
/** Compute-exclusive mode - Only one thread will be able to use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_EXCLUSIVE = "cudaComputeModeExclusive";
/** Compute-prohibited mode - No threads can use cudaSetDevice() with this device. @n
 * Any errors from calling cudaSetDevice() with an exclusive (and occupied) or prohibited device @n
 * will only show up after a non-device management runtime function is called. @n
 * At that time, cudaErrorNoDevice will be returned. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_PROHIBITED = "cudaComputeModeProhibited";

/** is 1 if the device supports executing multiple kernels within the same context simultaneously, or 0 if not. @n
 * It is not guaranteed that multiple kernels will be resident on the device concurrently @n
 * so this feature should not be relied upon for correctness; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CONCURRENT_KERNELS = "concurrentKernels";
/** is 1 if the device has ECC support turned on, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_ECC_ENABLED = "ECCEnabled";
/** is the PCI bus identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_BUS_ID = "pciBusID";
/** is the PCI device (sometimes called slot) identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_DEVICE_ID = "pciDeviceID";
/** is 1 if the device is using a TCC driver or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TCC_DRIVER = "tccDriver";

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

GpuPlatforms getPlatformList()
{
    return {GpuPlatform(GpuType::GT_CUDA, 0)};
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkCudaGpuType(platform);
    GpuPlatformInfo info;
    info.name   = "CUDA";
    info.vendor = "NVIDIA";
#if defined(USE_CUDA)
    int driver_version = 0;
    cudaError_t driver_code = ::cudaDriverGetVersion(&driver_version);
    if (driver_code == cudaSuccess) {
        info.version += string::fformat("DRIVER({})", driver_version);
    } else {
        tDLogE("getPlatformInfo() CUDA cudaDriverGetVersion() error: {}", ::cudaGetErrorString(driver_code));
    }

    int runtime_version = 0;
    cudaError_t runtime_code = ::cudaRuntimeGetVersion(&runtime_version);
    if (runtime_code == cudaSuccess) {
        info.version += string::fformat("RUNTIME({})", runtime_version);
    } else {
        tDLogE("getPlatformInfo() CUDA cudaRuntimeGetVersion() error: {}", ::cudaGetErrorString(runtime_code));
    }

# if defined(CUDA_VERSION)
    info.version += string::fformat("API({})", TO_STRING(CUDA_VERSION));
# endif
#endif
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkCudaGpuType(platform);
    int result = 0;
#if defined(USE_CUDA)
    cudaError_t code = ::cudaGetDeviceCount(&result);
    if (code != cudaSuccess) {
        tDLogE("getDeviceCount() CUDA cudaGetDeviceCount() error: {}", ::cudaGetErrorString(code));
        return 0;
    }
#endif
    return result;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkCudaGpuType(platform);
    GpuDevices result;
    for (int i = 0; i < getDeviceCount(platform); ++i) {
        result.emplace_back(GpuDevice(platform, i));
    }
    return result;
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkCudaGpuType(device);
    GpuDeviceInfo info;
#if defined(USE_CUDA)
    cudaDeviceProp prop;
    cudaError_t code = ::cudaGetDeviceProperties(&prop, device.device_id);
    if (code == cudaSuccess) {
        info.name           = prop.name;
        info.device_version = string::fformat("{}.{}", prop.major, prop.minor);
        info.global_memory  = prop.totalGlobalMem;

        int driver_version = 0;
        cudaError_t driver_code = ::cudaDriverGetVersion(&driver_version);
        if (driver_code == cudaSuccess) {
            info.driver_version = std::to_string(driver_version);
        } else {
            tDLogE("getDeviceInfo() CUDA cudaDriverGetVersion() error: {}", ::cudaGetErrorString(driver_code));
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
        if (prop.computeMode == ::cudaComputeModeExclusive) {
            info.insert(TBAG_GPU_DEVICE_INFO_COMPUTE_MODE, TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_EXCLUSIVE);
        } else if (prop.computeMode == ::cudaComputeModeProhibited) {
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
    } else {
        tDLogE("getDeviceInfo() CUDA cudaGetDeviceProperties() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkCudaGpuType(device);
    return SharedGpuContext(new CudaContext(device, 0));
}

// ---------------------------
// CudaContext implementation.
// ---------------------------

CudaContext::CudaContext(GpuDevice const & d, GpuId c) : GpuContext(d, c)
{
    // EMPTY.
}

CudaContext::~CudaContext()
{
    // EMPTY.
}

// @formatter:off
bool CudaContext::isSupport() const TBAG_NOEXCEPT { return cuda::isSupport(); }
bool CudaContext::isHost   () const TBAG_NOEXCEPT { return false; }
bool CudaContext::isDevice () const TBAG_NOEXCEPT { return  true; }
// @formatter:on

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

