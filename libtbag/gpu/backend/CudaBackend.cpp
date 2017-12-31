/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
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

// ---------------
namespace __impl {
// ---------------

static void startEvent(GpuQueue & queue, GpuEvent * event)
{
#if defined(USE_CUDA)
    cudaError_t code = ::cudaEventRecord((cudaEvent_t)event->start, (cudaStream_t)queue.queue_id);
    if (code != cudaSuccess) {
        tDLogW("startEvent() CUDA cudaEventRecord() error: {}", ::cudaGetErrorString(code));
    }
#endif
}

static void stopEvent(GpuQueue & queue, GpuEvent * event)
{
#if defined(USE_CUDA)
    cudaError_t code = ::cudaEventRecord((cudaEvent_t)event->stop, (cudaStream_t)queue.queue_id);
    if (code != cudaSuccess) {
        tDLogW("stopEvent() CUDA cudaEventRecord() error: {}", ::cudaGetErrorString(code));
    }
#endif
}

struct CudaEventGuard : private Noncopyable
{
    GpuQueue & queue;
    GpuEvent * event;

    CudaEventGuard(GpuQueue & q, GpuEvent * e = nullptr) : queue(q), event(e)
    {
        if (event != nullptr) {
            startEvent(queue, event);
        }
    }

    ~CudaEventGuard()
    {
        if (event != nullptr) {
            stopEvent(queue, event);
        }
    }
};

struct CudaDeviceGuard : private Noncopyable
{
    int  current_id;
    int   change_id;
    bool   exchange;

    CudaDeviceGuard(GpuDevice  const &  device) : CudaDeviceGuard( device.device_id) { /* EMPTY. */ }
    CudaDeviceGuard(GpuContext const & context) : CudaDeviceGuard(context.device_id) { /* EMPTY. */ }
    CudaDeviceGuard(GpuMemory  const &  memory) : CudaDeviceGuard( memory.device_id) { /* EMPTY. */ }
    CudaDeviceGuard(HostMemory const &  memory) : CudaDeviceGuard( memory.device_id) { /* EMPTY. */ }

    CudaDeviceGuard(GpuId const & id) : current_id(0), change_id(0), exchange(false)
    {
        if (id == UNKNOWN_GPU_ID) {
            return;
        }
#if defined(USE_CUDA)
        cudaError_t get_code = ::cudaGetDevice(&current_id);
        if (get_code != cudaSuccess) {
            tDLogE("CudaDeviceGuard::CudaDeviceGuard() CUDA cudaGetDevice() error: {}", ::cudaGetErrorString(get_code));
            return;
        }

        change_id = (int)id;
        if (current_id == change_id) {
            return;
        }

        cudaError_t set_code = ::cudaSetDevice(change_id);
        if (set_code == cudaSuccess) {
            exchange = true;
        } else {
            tDLogE("CudaDeviceGuard::CudaDeviceGuard() CUDA cudaSetDevice() error: {}", ::cudaGetErrorString(set_code));
        }
#endif
    }

    ~CudaDeviceGuard()
    {
        if (exchange == false) {
            return;
        }
#if defined(USE_CUDA)
        cudaError_t set_code = ::cudaSetDevice(current_id);
        if (set_code == cudaSuccess) {
            tDLogE("CudaDeviceGuard::~CudaDeviceGuard() CUDA cudaSetDevice() error: {}", ::cudaGetErrorString(set_code));
        }
#endif
    }
};

// ------------------
} // namespace __impl
// ------------------

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
        tDLogE("CudaBackend::getPlatformInfo() CUDA cudaDriverGetVersion() error: {}", ::cudaGetErrorString(driver_code));
    }

    int runtime_version = 0;
    cudaError_t runtime_code = ::cudaRuntimeGetVersion(&runtime_version);
    if (runtime_code == cudaSuccess) {
        info.version += string::fformat("RUNTIME({})", runtime_version);
    } else {
        tDLogE("CudaBackend::getPlatformInfo() CUDA cudaRuntimeGetVersion() error: {}", ::cudaGetErrorString(runtime_code));
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
        tDLogE("CudaBackend::getDeviceCount() CUDA cudaGetDeviceCount() error: {}", ::cudaGetErrorString(code));
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
        tDLogE("CudaBackend::getDeviceInfo() CUDA cudaGetDeviceProperties() error: {}", ::cudaGetErrorString(code));
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
        tDLogE("CudaBackend::createQueue() CUDA cudaStreamCreate() error: {}", ::cudaGetErrorString(code));
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
        tDLogE("CudaBackend::releaseQueue() CUDA cudaStreamDestroy() error: {}", ::cudaGetErrorString(code));
        return false;
    }
#endif
    return true;
}

GpuEvent CudaBackend::createEvent(GpuQueue const & queue) const
{
    GpuEvent result(queue);
#if defined(USE_CUDA)
    cudaError_t start_code = ::cudaEventCreate((cudaEvent_t*)&result.start);
    if (start_code != cudaSuccess) {
        tDLogE("CudaBackend::createEvent() CUDA cudaEventCreate(start) error: {}", ::cudaGetErrorString(start_code));
        return result;
    }

    cudaError_t stop_code = ::cudaEventCreate((cudaEvent_t*)&result.stop);
    if (stop_code != cudaSuccess) {
        ::cudaEventDestroy((cudaEvent_t)result.start);
        tDLogE("CudaBackend::createEvent() CUDA cudaEventCreate(stop) error: {}", ::cudaGetErrorString(stop_code));
        return result;
    }
#endif
    return result;
}

bool CudaBackend::syncEvent(GpuEvent const & event) const
{
#if defined(USE_CUDA)
    cudaError_t start_code = ::cudaEventSynchronize((cudaEvent_t)event.start);
    cudaError_t  stop_code = ::cudaEventSynchronize((cudaEvent_t)event.stop);
    if (start_code == cudaSuccess && stop_code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::syncEvent() CUDA cudaEventSynchronize() error: start({}), stop({})",
               ::cudaGetErrorString(start_code), ::cudaGetErrorString(stop_code));
    }
#endif
    return false;
}

bool CudaBackend::elapsedEvent(GpuEvent & event, float * millisec) const
{
#if defined(USE_CUDA)
    float elapsed_time = 0.0f;
    cudaError_t code = ::cudaEventElapsedTime(&elapsed_time,
                                              (cudaEvent_t)event.start,
                                              (cudaEvent_t)event.stop);
    if (code == cudaSuccess) {
        if (millisec != nullptr) {
            *millisec = elapsed_time;
        }
        return true;
    } else {
        tDLogW("stopEvent() CUDA cudaEventElapsedTime() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return false;
}

bool CudaBackend::releaseEvent(GpuEvent & event) const
{
#if defined(USE_CUDA)
    cudaError_t start_code = ::cudaEventDestroy((cudaEvent_t)event.start);
    cudaError_t  stop_code = ::cudaEventDestroy((cudaEvent_t)event.stop);
    event.start = UNKNOWN_GPU_ID;
    event.stop  = UNKNOWN_GPU_ID;
    if (start_code == cudaSuccess && stop_code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::releaseEvent() CUDA cudaEventDestroy() error: start({}), stop({})",
               ::cudaGetErrorString(start_code), ::cudaGetErrorString(stop_code));
    }
#endif
    return false;
}

GpuMemory CudaBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory memory(context);
    __impl::CudaDeviceGuard const LOCK(context);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMalloc((void**)&memory.data, size);
    if (code == cudaSuccess) {
        memory.size = size;
    } else {
        tDLogE("CudaBackend::malloc() CUDA cudaMalloc() error: {}", ::cudaGetErrorString(code));
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
    __impl::CudaDeviceGuard const LOCK(memory);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaFree(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    if (code != cudaSuccess) {
        tDLogE("CudaBackend::free() CUDA cudaFree() error: {}", ::cudaGetErrorString(code));
        return false;
    }
#endif
    return true;
}

HostMemory CudaBackend::mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag) const
{
    checkType(context.type);
    HostMemory result(context);
    if (HostMemoryFlag::HMF_DEFAULT != flag && HostMemoryFlag::HMF_PINNED != flag) {
        return result;
    }
    __impl::CudaDeviceGuard const LOCK(context);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMallocHost((void**)&result.data, size);
    if (code == cudaSuccess) {
        result.size = size;
        result.flag = flag;
    } else {
        tDLogE("CudaBackend::mallocHost() CUDA cudaMallocHost() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return result;
}

bool CudaBackend::freeHost(HostMemory & memory) const
{
    checkType(memory.type);
    __impl::CudaDeviceGuard const LOCK(memory);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaFreeHost(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    memory.flag = HostMemoryFlag::HMF_UNINITIALIZED;
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::free() CUDA cudaFreeHost() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return false;
}

bool CudaBackend::write(GpuQueue & queue, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CudaBackend::write() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(queue, event);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMemcpy(gpu_mem.data, host_mem.data, size, ::cudaMemcpyHostToDevice);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::write() CUDA cudaMemcpy() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return false;
}

bool CudaBackend::read(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CudaBackend::read() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(queue, event);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMemcpy(host_mem.data, gpu_mem.data, size, ::cudaMemcpyDeviceToHost);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::read() CUDA cudaMemcpy() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return false;
}

bool CudaBackend::enqueueWrite(GpuQueue & queue, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CudaBackend::enqueueWrite() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(queue, event);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMemcpyAsync(gpu_mem.data, host_mem.data, size, ::cudaMemcpyHostToDevice,
                                         (cudaStream_t)queue.queue_id);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::enqueueWrite() CUDA cudaMemcpyAsync() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return true;
}

bool CudaBackend::enqueueRead(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CudaBackend::enqueueRead() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CudaEventGuard const EVENT_LOCK(queue, event);
#if defined(USE_CUDA)
    cudaError_t code = ::cudaMemcpyAsync(host_mem.data, gpu_mem.data, size, ::cudaMemcpyDeviceToHost,
                                         (cudaStream_t)queue.queue_id);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::enqueueRead() CUDA cudaMemcpyAsync() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return true;
}

bool CudaBackend::flush(GpuQueue & queue) const
{
    return true;
}

bool CudaBackend::finish(GpuQueue & queue) const
{
#if defined(USE_CUDA)
    cudaError_t code = ::cudaStreamSynchronize((cudaStream_t)queue.queue_id);
    if (code == cudaSuccess) {
        return true;
    } else {
        tDLogE("CudaBackend::finish() CUDA cudaDeviceSynchronize() error: {}", ::cudaGetErrorString(code));
    }
#endif
    return false;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

