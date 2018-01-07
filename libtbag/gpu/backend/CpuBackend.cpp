/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/gpu/backend/GpuKernels.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdlib>
#include <chrono>

//#define TBAG_CPU_BACKEND_DEBUGGING

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

// ---------------
namespace __impl {
// ---------------

TBAG_CONSTEXPR static bool isCpuBackendVerbose() TBAG_NOEXCEPT
{
#if defined(TBAG_CPU_BACKEND_DEBUGGING)
    return true;
#else
    return false;
#endif
}

struct CpuContextBackend
{
    CpuContextBackend()
    {
        tDLogIfD(isCpuBackendVerbose(), "CpuContextBackend::CpuContextBackend() ID: @{}", (void*)this);
    }

    ~CpuContextBackend()
    {
        tDLogIfD(isCpuBackendVerbose(), "CpuContextBackend::~CpuContextBackend() ID: @{}", (void*)this);
    }
};

struct CpuQueueBackend
{
    CpuQueueBackend()
    {
        tDLogIfD(isCpuBackendVerbose(), "CpuQueueBackend::CpuQueueBackend() ID: @{}", (void*)this);
    }

    ~CpuQueueBackend()
    {
        tDLogIfD(isCpuBackendVerbose(), "CpuQueueBackend::~CpuQueueBackend() ID: @{}", (void*)this);
    }
};

struct CpuEventGuard : private Noncopyable
{
    GpuEvent * event;

    CpuEventGuard(GpuEvent * e = nullptr) : event(e)
    {
        if (event != nullptr) {
            event->start = now();
        }
    }

    ~CpuEventGuard()
    {
        if (event != nullptr) {
            event->stop = now();
        }
    }

    GpuId now() const
    {
        using namespace std::chrono;
        return (GpuId)duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    }
};

// ------------------
} // namespace __impl
// ------------------

GpuBackendType CpuBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_CPU;
}

bool CpuBackend::isSupport() const TBAG_NOEXCEPT
{
    return true;
}

bool CpuBackend::isHost() const TBAG_NOEXCEPT
{
    return true;
}

bool CpuBackend::isDevice() const TBAG_NOEXCEPT
{
    return false;
}

int CpuBackend::getPlatformCount() const
{
    return 1;
}

GpuPlatforms CpuBackend::getPlatformList() const
{
    return {GpuPlatform(getType(), 0)};
}

GpuPlatformInfo CpuBackend::getPlatformInfo(GpuPlatform const & platform) const
{
    checkType(platform.type);
    GpuPlatformInfo info(platform);
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int CpuBackend::getDeviceCount(GpuPlatform const & platform) const
{
    checkType(platform.type);
    return 1;
}

GpuDevices CpuBackend::getDeviceList(GpuPlatform const & platform) const
{
    checkType(platform.type);
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo CpuBackend::getDeviceInfo(GpuDevice const & device) const
{
    checkType(device.type);
    GpuDeviceInfo info(device);
    info.name = "CPU";
    return info;
}

GpuContext CpuBackend::createContext(GpuDevice const & device) const
{
    checkType(device.type);
    GpuContext result(device);
    auto * context = new __impl::CpuContextBackend();
    result.context_id = (GpuId)context;
    return result;
}

bool CpuBackend::releaseContext(GpuContext & context) const
{
    checkType(context.type);
    if (context.isUnknownContext()) {
        tDLogE("CpuBackend::releaseContext() Illegal stream.");
        return false;
    }
    delete ((__impl::CpuContextBackend*)context.context_id);
    context.context_id = UNKNOWN_GPU_ID;
    return true;
}

bool CpuBackend::isStreamSupported(GpuDevice const & device) const
{
    return true;
}

GpuStream CpuBackend::createStream(GpuContext const & context) const
{
    checkType(context.type);
    GpuStream result(context);
    if (context.isUnknownContext()) {
        tDLogE("CpuBackend::createStream() Illegal stream.");
        return result;
    }
    auto * stream = new __impl::CpuQueueBackend();
    result.stream_id = (GpuId)stream;
    return result;
}

bool CpuBackend::releaseStream(GpuStream & stream) const
{
    checkType(stream.type);
    if (stream.isUnknownStream()) {
        tDLogE("CpuBackend::releaseStream() Illegal stream.");
        return false;
    }
    delete ((__impl::CpuQueueBackend*)stream.stream_id);
    stream.stream_id = UNKNOWN_GPU_ID;
    return true;
}

GpuEvent CpuBackend::createEvent(GpuStream const & stream) const
{
    checkType(stream.type);
    GpuEvent result(stream);
    result.start = 0;
    result.stop  = 0;
    return result;
}

bool CpuBackend::syncEvent(GpuEvent const & event) const
{
    checkType(event.type);
    if (event.isUnknownEvent()) {
        return false;
    }
    return true;
}

bool CpuBackend::elapsedEvent(GpuEvent & event, float * millisec) const
{
    checkType(event.type);
    if (event.isUnknownEvent()) {
        return false;
    }
    if (millisec != nullptr) {
        *millisec = (event.stop - event.start) * 1.0e-3f;
    }
    return true;
}

bool CpuBackend::releaseEvent(GpuEvent & event) const
{
    checkType(event.type);
    event.start = UNKNOWN_GPU_ID;
    event.stop  = UNKNOWN_GPU_ID;
    return true;
}

GpuMemory CpuBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory memory(context);
    memory.data = ::malloc(size);
    memory.size = size;
    return memory;
}

bool CpuBackend::free(GpuMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("CpuBackend::free() Illegal memory.");
        return false;
    }
    ::free(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    return true;
}

HostMemory CpuBackend::mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag) const
{
    checkType(context.type);
    HostMemory memory(context);
    memory.data = ::malloc(size);
    memory.size = size;
    memory.flag = flag;
    return memory;
}

bool CpuBackend::freeHost(HostMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("CpuBackend::freeHost() Illegal memory.");
        return false;
    }
    ::free(memory.data);
    memory.data = nullptr;
    memory.size = 0;
    memory.flag = HostMemoryFlag::HMF_UNINITIALIZED;
    return true;
}

bool CpuBackend::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::write() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(gpu_mem.data, host_mem.data, size);
    return true;
}

bool CpuBackend::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::read() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(host_mem.data, gpu_mem.data, size);
    return true;
}

bool CpuBackend::enqueueWrite(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::enqueueWrite() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(gpu_mem.data, host_mem.data, size);
    return true;
}

bool CpuBackend::enqueueRead(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::enqueueRead() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }

    __impl::CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(host_mem.data, gpu_mem.data, size);
    return true;
}

bool CpuBackend::flush(GpuStream & stream) const
{
    return true;
}

bool CpuBackend::finish(GpuStream & stream) const
{
    return true;
}

bool CpuBackend::runAdd(GpuStream & stream, GpuMemory const & v1, GpuMemory const & v2, GpuMemory & result,
                        type::TypeTable type, int count) const
{
    checkType(stream.type);
    checkType(v1.type);
    checkType(v2.type);
    checkType(result.type);

    if (type == type::TypeTable::TT_FLOAT) {
        kernels::addByCpu((float const *)v1.data, (float const *)v2.data, (float *)result.data, count);
    } else if (type == type::TypeTable::TT_DOUBLE) {
        kernels::addByCpu((double const *)v1.data, (double const *)v2.data, (double *)result.data, count);
    } else {
        return false;
    }
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

