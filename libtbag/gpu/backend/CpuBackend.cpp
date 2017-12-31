/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdlib>
#include <queue>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

// ---------------
namespace __impl {
// ---------------

struct CpuContextBackend
{
    bool verbose;

    CpuContextBackend(bool v = false) : verbose(v)
    {
        tDLogIfD(verbose, "CpuContextBackend::CpuContextBackend() ID: @{}", (void*)this);
    }

    ~CpuContextBackend()
    {
        tDLogIfD(verbose, "CpuContextBackend::~CpuContextBackend() ID: @{}", (void*)this);
    }
};

struct CpuQueueBackend
{
    bool verbose;

    CpuQueueBackend(bool v = false) : verbose(v)
    {
        tDLogIfD(verbose, "CpuQueueBackend::CpuQueueBackend() ID: @{}", (void*)this);
    }

    ~CpuQueueBackend()
    {
        tDLogIfD(verbose, "CpuQueueBackend::~CpuQueueBackend() ID: @{}", (void*)this);
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
        tDLogE("CpuBackend::releaseContext() Illegal queue.");
        return false;
    }
    delete ((__impl::CpuContextBackend*)context.context_id);
    context.context_id = UNKNOWN_GPU_ID;
    return true;
}

GpuQueue CpuBackend::createQueue(GpuContext const & context) const
{
    checkType(context.type);
    GpuQueue result(context);
    if (context.isUnknownContext()) {
        tDLogE("CpuBackend::releaseContext() Illegal queue.");
        return result;
    }
    auto * queue = new __impl::CpuQueueBackend((__impl::CpuContextBackend*)context.context_id);
    result.queue_id = (GpuId)queue;
    return result;
}

bool CpuBackend::releaseQueue(GpuQueue & queue) const
{
    checkType(queue.type);
    if (queue.isUnknownQueue()) {
        tDLogE("CpuBackend::releaseQueue() Illegal queue.");
        return false;
    }
    delete ((__impl::CpuQueueBackend*)queue.queue_id);
    queue.queue_id = UNKNOWN_GPU_ID;
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

bool CpuBackend::write(GpuQueue & queue, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::write() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    ::memcpy(gpu_mem.data, host_mem.data, size);
    return true;
}

bool CpuBackend::read(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::read() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    ::memcpy(host_mem.data, gpu_mem.data, size);
    return true;
}

bool CpuBackend::enqueueWrite(GpuQueue & queue, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::enqueueWrite() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    ::memcpy(gpu_mem.data, host_mem.data, size);
    return true;
}

bool CpuBackend::enqueueRead(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("CpuBackend::enqueueRead() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    ::memcpy(host_mem.data, gpu_mem.data, size);
    return true;
}

bool CpuBackend::flush(GpuQueue & queue) const
{
    return true;
}

bool CpuBackend::finish(GpuQueue & queue) const
{
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

