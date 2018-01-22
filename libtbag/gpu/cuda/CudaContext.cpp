/**
 * @file   CudaContext.cpp
 * @brief  CudaContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/cuda/CudaContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/gpu/cuda/CudaRaw.h>

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

// ---------------
namespace __impl {
// ---------------

static void startEvent(GpuStream const & stream, GpuEvent * event)
{
    cudaError_t code = cudaEventRecord(event->castStart<cudaEvent_t>(), stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogW("startEvent() CUDA cudaEventRecord() error: {}", cudaGetErrorString(code));
    }
}

static void stopEvent(GpuStream const & stream, GpuEvent * event)
{
    cudaError_t code = cudaEventRecord(event->castStop<cudaEvent_t>(), stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogW("stopEvent() CUDA cudaEventRecord() error: {}", cudaGetErrorString(code));
    }
}

struct CudaEventGuard : private Noncopyable
{
    GpuStream const & stream;
    GpuEvent * event;

    CudaEventGuard(GpuStream const & q, GpuEvent * e = nullptr) : stream(q), event(e)
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

    explicit CudaDeviceGuard(GpuDevice  const &  device) : CudaDeviceGuard( device.getDeviceId()) { /* EMPTY. */ }
    explicit CudaDeviceGuard(GpuContext const & context) : CudaDeviceGuard(context.getDeviceId()) { /* EMPTY. */ }

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
    cudaError_t code = cudaGetDeviceProperties(&prop, device.getDeviceId());
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
    cudaDeviceProp prop;
    cudaError_t code = cudaGetDeviceProperties(&prop, castDeviceId<int>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::isStream() CUDA cudaGetDeviceProperties() error: {}", cudaGetErrorString(code));
        return false;
    }
    return prop.deviceOverlap == 1;
}

Err CudaContext::createStream(GpuStream & stream) const
{
    if (stream.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaStream_t native_stream;
    cudaError_t code = cudaStreamCreate(&native_stream);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::createStream() CUDA cudaStreamCreate() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }

    stream.setId(native_stream);
    return Err::E_SUCCESS;
}

Err CudaContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaError_t code = cudaStreamDestroy(stream.castId<cudaStream_t>());
    stream.clearId();
    if (code != cudaSuccess) {
        tDLogE("CudaContext::releaseStream() CUDA cudaStreamDestroy() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(*this) == false || event.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaEvent_t native_start;
    cudaError_t start_code = cudaEventCreate(&native_start);
    if (start_code != cudaSuccess) {
        tDLogE("CudaContext::createEvent() CUDA cudaEventCreate() start error: {}", cudaGetErrorString(start_code));
        return Err::E_CUDA;
    }

    cudaEvent_t native_stop;
    cudaError_t stop_code = cudaEventCreate(&native_stop);
    if (stop_code != cudaSuccess) {
        cudaEventDestroy(native_start);
        tDLogE("CudaContext::createEvent() CUDA cudaEventCreate() stop error: {}", cudaGetErrorString(stop_code));
        return Err::E_CUDA;
    }

    event.setStart(native_start);
    event.setStop(native_stop);
    return Err::E_SUCCESS;
}

Err CudaContext::syncEvent(GpuEvent const & event) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaError_t start_code = cudaEventSynchronize(event.castStart<cudaEvent_t>());
    cudaError_t  stop_code = cudaEventSynchronize(event.castStop<cudaEvent_t>());
    if (start_code != cudaSuccess || stop_code != cudaSuccess) {
        tDLogE("CudaContext::syncEvent() CUDA cudaEventSynchronize() error: start({}), stop({})",
               cudaGetErrorString(start_code), cudaGetErrorString(stop_code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    float elapsed_time = 0.0f;
    cudaError_t code = cudaEventElapsedTime(&elapsed_time, event.castStart<cudaEvent_t>(), event.castStop<cudaEvent_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::elapsedEvent() CUDA cudaEventElapsedTime() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }

    if (millisec != nullptr) {
        *millisec = elapsed_time;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::releaseEvent(GpuEvent & event) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaError_t start_code = cudaEventDestroy(event.castStart<cudaEvent_t>());
    cudaError_t  stop_code = cudaEventDestroy(event.castStop<cudaEvent_t>());
    event.clearIds();
    if (start_code != cudaSuccess || stop_code != cudaSuccess) {
        tDLogE("CudaContext::releaseEvent() CUDA cudaEventDestroy() error: start({}), stop({})",
               cudaGetErrorString(start_code), cudaGetErrorString(stop_code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaDeviceGuard const LOCK(*this);
    void * data = nullptr;
    cudaError_t code = cudaMalloc(&data, size);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::malloc({}) CUDA cudaMalloc() error: {}", size, cudaGetErrorString(code));
        return Err::E_CUDA;
    }

    memory.set(data, size, size);
    tDLogIfD(isGpuVerbose(), "CudaContext::malloc({}) CUDA cudaMalloc() MEM:{} CAP:{} SIZE:{}",
             size, memory.data(), memory.capacity(), memory.size());
    return Err::E_SUCCESS;
}

Err CudaContext::free(GpuMemory & memory) const
{
    if (memory.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "CudaContext::free() CUDA cudaFree() MEM:{} CAP:{} SIZE:{}",
             memory.data(), memory.capacity(), memory.size());

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaFree(memory.data());
    memory.clear();
    if (code != cudaSuccess) {
        tDLogE("CudaContext::free() CUDA cudaFree() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag) const
{
    if (memory.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }
    if (HostMemoryFlag::HMF_DEFAULT == flag) {
        return GpuContext::mallocHost(memory, size, HostMemoryFlag::HMF_DEFAULT);
    }
    if (HostMemoryFlag::HMF_PINNED != flag) {
        tDLogE("CudaContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return Err::E_ILLARGS;
    }
    assert(flag == HostMemoryFlag::HMF_PINNED);

    __impl::CudaDeviceGuard const LOCK(*this);
    void * data = nullptr;
    cudaError_t code = cudaMallocHost(&data, size);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::mallocHost({}) CUDA cudaMallocHost() error: {}", size, cudaGetErrorString(code));
        return Err::E_CUDA;
    }

    memory.set(data, size, size, flag);
    tDLogIfD(isGpuVerbose(), "CudaContext::mallocHost({}) CUDA cudaMallocHost() MEM:{} CAP:{} SIZE:{}",
             size, memory.data(), memory.capacity(), memory.size());
    return Err::E_SUCCESS;
}

Err CudaContext::freeHost(HostMemory & memory) const
{
    if (memory.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "CudaContext::freeHost() CUDA cudaFreeHost() MEM:{} CAP:{} SIZE:{}",
             memory.data(), memory.capacity(), memory.size());

    __impl::CudaDeviceGuard const LOCK(*this);
    cudaError_t code = cudaFreeHost(memory.data());
    memory.clear();
    if (code != cudaSuccess) {
        tDLogE("CudaContext::free() CUDA cudaFreeHost() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::write(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(gpu_mem.data(), host_mem.data(), size, cudaMemcpyHostToDevice);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::write() CUDA cudaMemcpy() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(host_mem.data(), gpu_mem.data(), size, cudaMemcpyDeviceToHost);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::read() CUDA cudaMemcpy() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::writeAsync(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(gpu_mem.data(), host_mem.data(), size, cudaMemcpyHostToDevice,
                                       stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::writeAsync() CUDA cudaMemcpyAsync() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::readAsync(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(host_mem.data(), gpu_mem.data(), size, cudaMemcpyDeviceToHost,
                                       stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::readAsync() CUDA cudaMemcpyAsync() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::copy(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(dest.data(), src.data(), size, cudaMemcpyDeviceToDevice);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::copy() CUDA cudaMemcpy(D2D) error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::copy(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpy(dest.data(), src.data(), size, cudaMemcpyHostToHost);
    if (code != cudaSuccess) {
        tDLogE("CudaContext::copy() CUDA cudaMemcpy(H2H) error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::copyAsync(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(dest.data(), src.data(), size, cudaMemcpyDeviceToDevice,
                                       stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::copyAsync() CUDA cudaMemcpyAsync(D2D) error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::copyAsync(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    __impl::CudaEventGuard const EVENT_LOCK(stream, event);
    cudaError_t code = cudaMemcpyAsync(dest.data(), src.data(), size, cudaMemcpyHostToHost,
                                       stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::copyAsync() CUDA cudaMemcpyAsync(H2H) error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::flush(GpuStream const & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::finish(GpuStream const & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cudaError_t code = cudaStreamSynchronize(stream.castId<cudaStream_t>());
    if (code != cudaSuccess) {
        tDLogE("CudaContext::finish() CUDA cudaDeviceSynchronize() error: {}", cudaGetErrorString(code));
        return Err::E_CUDA;
    }
    return Err::E_SUCCESS;
}

Err CudaContext::add(GpuStream const & stream, int const * in1, int const * in2, int * out, int count, GpuEvent * event) const
{ __impl::CudaEventGuard g(stream, event); return tbCudaAdd_i(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err CudaContext::add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const
{ __impl::CudaEventGuard g(stream, event); return tbCudaAdd_u(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err CudaContext::add(GpuStream const & stream, float const * in1, float const * in2, float * out, int count, GpuEvent * event) const
{ __impl::CudaEventGuard g(stream, event); return tbCudaAdd_f(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err CudaContext::add(GpuStream const & stream, double const * in1, double const * in2, double * out, int count, GpuEvent * event) const
{ __impl::CudaEventGuard g(stream, event); return tbCudaAdd_d(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

