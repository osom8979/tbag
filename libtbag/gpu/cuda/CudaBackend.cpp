/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2018-01-13
 */

#include <libtbag/gpu/cuda/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>

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

// ---------------
namespace __impl {
// ---------------

static void startEvent(GpuStream & stream, GpuEvent * event)
{
    cudaError_t code = cudaEventRecord((cudaEvent_t)event->start, stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogW("startEvent() CUDA cudaEventRecord() error: {}", cudaGetErrorString(code));
    }
}

static void stopEvent(GpuStream & stream, GpuEvent * event)
{
    cudaError_t code = cudaEventRecord((cudaEvent_t)event->stop, stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogW("stopEvent() CUDA cudaEventRecord() error: {}", cudaGetErrorString(code));
    }
}

struct CudaEventGuard : private Noncopyable
{
    GpuStream & stream;
    GpuEvent * event;

    CudaEventGuard(GpuStream & q, GpuEvent * e = nullptr) : stream(q), event(e)
    {
        if (event != nullptr) {
            startEvent(stream, event);
        }
    }

    ~CudaEventGuard()
    {
        if (event != nullptr) {
            stopEvent(stream, event);
        }
    }
};

struct CudaDeviceGuard : private Noncopyable
{
    int  current_id = 0;
    int   change_id = 0;
    bool   exchange = false;

    explicit CudaDeviceGuard(GpuDevice  const &  device) : CudaDeviceGuard( device.DEVICE) { /* EMPTY. */ }
    explicit CudaDeviceGuard(GpuContext const & context) : CudaDeviceGuard(context.DEVICE) { /* EMPTY. */ }

    CudaDeviceGuard(GpuId const & device_id) : current_id(0), change_id(0), exchange(false)
    {
        if (device_id == UNKNOWN_ID) {
            return;
        }
        cudaError_t get_code = cudaGetDevice(&current_id);
        if (get_code != cudaSuccess) {
            tDLogE("CudaDeviceGuard::CudaDeviceGuard() CUDA cudaGetDevice() error: {}", cudaGetErrorString(get_code));
            return;
        }

        change_id = (int)device_id;
        if (current_id == change_id) {
            return;
        }

        cudaError_t set_code = cudaSetDevice(change_id);
        if (set_code == cudaSuccess) {
            exchange = true;
        } else {
            tDLogE("CudaDeviceGuard::CudaDeviceGuard() CUDA cudaSetDevice() error: {}", cudaGetErrorString(set_code));
        }
    }

    ~CudaDeviceGuard()
    {
        if (exchange == false) {
            return;
        }
        cudaError_t set_code = cudaSetDevice(current_id);
        if (set_code == cudaSuccess) {
            tDLogE("CudaDeviceGuard::~CudaDeviceGuard() CUDA cudaSetDevice() error: {}", cudaGetErrorString(set_code));
        }
    }
};

// ------------------
} // namespace __impl
// ------------------

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
    info.version += string::fformat("API({})", TO_STRING(CUDA_VERSION));
#endif
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkCudaGpuType(platform);
    int result = 0;
    cudaError_t code = cudaGetDeviceCount(&result);
    if (code != cudaSuccess) {
        tDLogE("getDeviceCount() CUDA cudaGetDeviceCount() error: {}", cudaGetErrorString(code));
        return 0;
    }
    return result;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkCudaGpuType(platform);
    GpuDevices result;
    for (int i = 0; i < cuda::getDeviceCount(platform); ++i) {
        result.emplace_back(GpuDevice(platform, i));
    }
    return result;
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkCudaGpuType(device);
    GpuDeviceInfo info;
    cudaDeviceProp prop;
    cudaError_t code = cudaGetDeviceProperties(&prop, device.DEVICE);
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

bool CudaContext::isStream() const TBAG_NOEXCEPT
{
    auto const VALUE = DEVICE_INFO.get(TBAG_GPU_DEVICE_INFO_DEVICE_OVERLAP);
    if (VALUE.empty()) {
        return false;
    }
    return string::toValue<int>(VALUE) == 1;
}

bool CudaContext::createStream(GpuStream & stream) const
{
    if (stream.getContextPtr() != this) {
        tDLogE("CudaContext::createStream() Illegal stream argument.");
        return false;
    }

    cudaStream_t native_stream;
    cudaError_t code = cudaStreamCreate(&native_stream);
    if (code == cudaSuccess) {
        stream.setId(native_stream);
        return true;
    } else {
        tDLogE("CudaContext::createStream() CUDA cudaStreamCreate() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("CudaContext::releaseStream() Illegal stream argument.");
        return false;
    }

    cudaError_t code = cudaStreamDestroy(stream.castId<cudaStream_t>());
    stream.clearId();
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::releaseStream() CUDA cudaStreamDestroy() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(this) == false) {
        tDLogE("CudaContext::createEvent() Illegal stream argument.");
        return false;
    }
    if (event.CONTEXT != this) {
        tDLogE("CudaContext::createEvent() Illegal event argument.");
        return false;
    }

    cudaEvent_t native_start;
    cudaError_t start_code = cudaEventCreate(&native_start);
    if (start_code != cudaSuccess) {
        tDLogE("CudaContext::createEvent() CUDA cudaEventCreate(start) error: {}", cudaGetErrorString(start_code));
        return false;
    }

    cudaEvent_t native_stop;
    cudaError_t stop_code = cudaEventCreate(&native_stop);
    if (stop_code != cudaSuccess) {
        cudaEventDestroy(native_start);
        tDLogE("CudaContext::createEvent() CUDA cudaEventCreate(stop) error: {}", cudaGetErrorString(stop_code));
        return false;
    }

    event.start = (GpuId)native_start;
    event.stop  = (GpuId)native_stop;
    return true;
}

bool CudaContext::syncEvent(GpuEvent const & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("CudaContext::syncEvent() Illegal event argument.");
        return false;
    }

    cudaError_t start_code = cudaEventSynchronize((cudaEvent_t)event.start);
    cudaError_t  stop_code = cudaEventSynchronize((cudaEvent_t)event.stop);
    if (start_code == cudaSuccess && stop_code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::syncEvent() CUDA cudaEventSynchronize() error: start({}), stop({})",
               cudaGetErrorString(start_code), cudaGetErrorString(stop_code));
    }
    return false;
}

bool CudaContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("CudaContext::elapsedEvent() Illegal event argument.");
        return false;
    }

    float elapsed_time = 0.0f;
    cudaError_t code = cudaEventElapsedTime(&elapsed_time, (cudaEvent_t)event.start, (cudaEvent_t)event.stop);
    if (code == cudaSuccess) {
        if (millisec != nullptr) {
            *millisec = elapsed_time;
        }
        return true;
    } else {
        tDLogE("CudaContext::elapsedEvent() CUDA cudaEventElapsedTime() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::releaseEvent(GpuEvent & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("CudaContext::releaseEvent() Illegal event argument.");
        return false;
    }

    cudaError_t start_code = cudaEventDestroy((cudaEvent_t)event.start);
    cudaError_t  stop_code = cudaEventDestroy((cudaEvent_t)event.stop);
    event.start = UNKNOWN_ID;
    event.stop  = UNKNOWN_ID;
    if (start_code == cudaSuccess && stop_code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::releaseEvent() CUDA cudaEventDestroy() error: start({}), stop({})",
               cudaGetErrorString(start_code), cudaGetErrorString(stop_code));
    }
    return false;
}

bool CudaContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.CONTEXT != this) {
        tDLogE("CudaContext::malloc() Illegal memory arguments.");
        return false;
    }

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaMalloc(&memory.data, size);
    if (code == cudaSuccess) {
        memory.capacity = size;
        memory.size     = size;
        tDLogIfD(isGpuVerbose(), "CudaContext::malloc({}) CUDA cudaMalloc() MEM:{} CAP:{} SIZE:{}",
                 size, memory.data, memory.capacity , memory.size);
        return true;
    } else {
        tDLogE("CudaContext::malloc({}) CUDA cudaMalloc() error: {}", size, cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::free(GpuMemory & memory) const
{
    if (memory.CONTEXT != this || memory.exists() == false) {
        tDLogE("CudaContext::free() Illegal memory.");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "CudaContext::free() CUDA cudaFree() MEM:{} SIZE:{}",
             memory.data, memory.size);

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaFree(memory.data);
    memory.data     = nullptr;
    memory.capacity = 0;
    memory.size     = 0;
    if (code != cudaSuccess) {
        tDLogE("CudaContext::free() CUDA cudaFree() error: {}", cudaGetErrorString(code));
        return false;
    }
    return true;
}

bool CudaContext::mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag) const
{
    if (memory.CONTEXT != this) {
        tDLogE("CudaContext::mallocHost() Illegal memory argument");
        return false;
    }

    if (HostMemoryFlag::HMF_DEFAULT == flag) {
        return GpuContext::mallocHost(memory, size, flag);
    }
    if (HostMemoryFlag::HMF_PINNED != flag) {
        tDLogE("CudaContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return false;
    }
    assert(flag == HostMemoryFlag::HMF_PINNED);

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaMallocHost(&memory.data, size);
    if (code == cudaSuccess) {
        memory.capacity = size;
        memory.size     = size;
        memory.flag     = flag;
        tDLogIfD(isGpuVerbose(), "CudaContext::mallocHost({}) CUDA cudaMallocHost() MEM:{} CAP:{} SIZE:{}",
                 size, memory.data, memory.capacity, memory.size);
        return true;
    } else {
        tDLogE("CudaContext::mallocHost({}) CUDA cudaMallocHost() error: {}", size, cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::freeHost(HostMemory & memory) const
{
    if (memory.CONTEXT != this || memory.exists() == false) {
        tDLogE("CudaContext::freeHost() Illegal memory argument.");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "CudaContext::freeHost() CUDA cudaFreeHost() MEM:{} SIZE:{}",
             memory.data, memory.size);

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaFreeHost(memory.data);
    memory.data     = nullptr;
    memory.capacity = 0;
    memory.size     = 0;
    memory.flag     = HostMemoryFlag::HMF_DEFAULT;
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::free() CUDA cudaFreeHost() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("CudaContext::write() Illegal arguments.");
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(gpu_mem.data, host_mem.data, size, cudaMemcpyHostToDevice);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::write() CUDA cudaMemcpy() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("CudaContext::read() Illegal arguments.");
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(host_mem.data, gpu_mem.data, size, cudaMemcpyDeviceToHost);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::read() CUDA cudaMemcpy() error: {}", cudaGetErrorString(code));
    }
    return false;
}

bool CudaContext::writeAsync(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("CudaContext::writeAsync() Illegal arguments.");
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(gpu_mem.data, host_mem.data, size, cudaMemcpyHostToDevice,
                                       stream.castId<cudaStream_t>());
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::writeAsync() CUDA cudaMemcpyAsync() error: {}", cudaGetErrorString(code));
    }
    return true;
}

bool CudaContext::readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("CudaContext::readAsync() Illegal arguments.");
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(host_mem.data, gpu_mem.data, size, cudaMemcpyDeviceToHost,
                                       stream.castId<cudaStream_t>());
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::readAsync() CUDA cudaMemcpyAsync() error: {}", cudaGetErrorString(code));
    }
    return true;
}

bool CudaContext::flush(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("CudaContext::flush() Illegal stream argument.");
        return false;
    }
    return true;
}

bool CudaContext::finish(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("CudaContext::finish() Illegal stream argument.");
        return false;
    }

    cudaError_t code = cudaStreamSynchronize(stream.castId<cudaStream_t>());
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaContext::finish() CUDA cudaDeviceSynchronize() error: {}", cudaGetErrorString(code));
    }
    return false;
}

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

