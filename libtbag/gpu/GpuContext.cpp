/**
 * @file   GpuContext.cpp
 * @brief  GpuContext class implementation.
 * @author zer0
 * @date   2018-01-14
 */

#include <libtbag/gpu/GpuContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/algorithm/Pack.hpp>

#include <libtbag/gpu/accel/AccelBackend.hpp>
#include <libtbag/gpu/cpu/CpuBackend.hpp>
#include <libtbag/gpu/cuda/CudaBackend.hpp>
#include <libtbag/gpu/opencl/OpenCLBackend.hpp>

#include <cstdlib>
#include <cassert>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

// --------------------------
// GpuContext implementation.
// --------------------------

GpuContext::GpuContext(GpuDevice const & d, GpuId c)
        : GpuDevice(d), CONTEXT(c),
          PLATFORM_INFO(libtbag::gpu::getPlatformInfo(GpuDevice(d))),
          DEVICE_INFO(libtbag::gpu::getDeviceInfo(GpuDevice(d)))
{
    // EMPTY.
}

GpuContext::~GpuContext()
{
    // EMPTY.
}

bool GpuContext::checkMemory(GpuStream const & stream, GpuMemory const & gpu_mem,
                             HostMemory const & host_mem, std::size_t size) const TBAG_NOEXCEPT
{
    return stream.validate(this)
           && gpu_mem.CONTEXT == this && gpu_mem.exists() && size <= gpu_mem.size
           && host_mem.CONTEXT == this && host_mem.exists() && size <= host_mem.size;
}

bool GpuContext::createStream(GpuStream & stream) const
{
    if (stream.getContextPtr() != this) {
        tDLogE("GpuContext::createStream() Illegal stream argument.");
        return false;
    }
    stream.setId(0);
    return true;
}

bool GpuContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("GpuContext::releaseStream() Illegal stream argument.");
        return false;
    }
    stream.clearId();
    return true;
}

bool GpuContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(this) == false) {
        tDLogE("GpuContext::createEvent() Illegal stream argument.");
        return false;
    }
    if (event.CONTEXT != this) {
        tDLogE("GpuContext::createEvent() Illegal event argument.");
        return false;
    }
    event.start = 0;
    event.stop  = 0;
    return true;
}

bool GpuContext::syncEvent(GpuEvent const & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("GpuContext::syncEvent() Illegal event argument.");
        return false;
    }
    return true;
}

bool GpuContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("GpuContext::elapsedEvent() Illegal event argument.");
        return false;
    }
    if (millisec != nullptr) {
        *millisec = (event.stop - event.start) * 1.0e-6f;
    }
    return true;
}

bool GpuContext::releaseEvent(GpuEvent & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("GpuContext::releaseEvent() Illegal event argument.");
        return false;
    }
    event.start = UNKNOWN_ID;
    event.stop  = UNKNOWN_ID;
    return true;
}

bool GpuContext::createProgram(std::string const & source, GpuProgram & program) const
{
    if (source.empty()) {
        tDLogE("GpuContext::createProgram() Empty source.");
        return false;
    }
    if (program.CONTEXT != this) {
        tDLogE("GpuContext::createProgram() Illegal program argument.");
        return false;
    }

    program.program = 0;
    return true;
}

bool GpuContext::buildProgram(GpuProgram & program) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("GpuContext::buildProgram() Illegal program argument.");
        return false;
    }
    return true;
}

bool GpuContext::releaseProgram(GpuProgram & program) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("GpuContext::releaseProgram() Illegal program argument.");
        return false;
    }
    program.program = UNKNOWN_ID;
    return true;
}

bool GpuContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("GpuContext::createKernel() Illegal program arguments.");
        return false;
    }
    if (kernel_symbol.empty()) {
        tDLogE("GpuContext::createKernel() Empty kernel symbol.");
        return false;
    }
    if (kernel.CONTEXT != this) {
        tDLogE("GpuContext::createKernel() Illegal kernel arguments.");
        return false;
    }

    kernel.kernel = 0;
    return true;
}

bool GpuContext::releaseKernel(GpuKernel & kernel) const
{
    if (kernel.CONTEXT != this || kernel.exists() == false) {
        tDLogE("GpuContext::releaseKernel() Illegal kernel context.");
        return false;
    }
    kernel.kernel = UNKNOWN_ID;
    return true;
}

bool GpuContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.CONTEXT != this) {
        tDLogE("GpuContext::malloc() Illegal memory arguments.");
        return false;
    }

    auto const  ALIGNED_SIZE = memory::getDefaultAlignedSize();
    auto const   PACKED_SIZE = algorithm::getPackedSize(size, ALIGNED_SIZE);
    auto const CAPACITY_SIZE = PACKED_SIZE * ALIGNED_SIZE;
    assert(CAPACITY_SIZE >= size);

    memory.data     = memory::alignedMemoryAlloc(CAPACITY_SIZE, ALIGNED_SIZE);
    memory.capacity = CAPACITY_SIZE;
    memory.size     = size;
    tDLogIfD(isGpuVerbose(), "GpuContext::malloc({}) Aligned malloc MEM:{} CAP:{} SIZE:{}",
             size, memory.data, memory.capacity, memory.size);

    return true;
}

bool GpuContext::free(GpuMemory & memory) const
{
    if (memory.CONTEXT != this || memory.exists() == false) {
        tDLogE("GpuContext::free() Illegal memory argument");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "GpuContext::free() Aligned free MEM:{} CAP:{} SIZE:{}",
             memory.data, memory.capacity, memory.size);

    memory::alignedMemoryFree(memory.data);
    memory.data     = nullptr;
    memory.capacity = 0;
    memory.size     = 0;
    return true;
}

bool GpuContext::mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag) const
{
    if (memory.CONTEXT != this) {
        tDLogE("GpuContext::mallocHost() Illegal memory argument");
        return false;
    }
    if (HostMemoryFlag::HMF_DEFAULT != flag) {
        tDLogE("GpuContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return false;
    }

    assert(flag == HostMemoryFlag::HMF_DEFAULT);
    return GpuContext::malloc(memory, size);
}

bool GpuContext::freeHost(HostMemory & memory) const
{
    if (memory.CONTEXT != this || memory.exists() == false) {
        tDLogE("GpuContext::freeHost() Illegal memory argument");
        return false;
    }
    return GpuContext::free(memory);
}

bool GpuContext::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("GpuContext::write() Illegal arguments.");
        return false;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(gpu_mem.data, host_mem.data, size);
    return true;
}

bool GpuContext::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("GpuContext::read() Illegal arguments.");
        return false;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(host_mem.data, gpu_mem.data, size);
    return true;
}

bool GpuContext::writeAsync(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    return GpuContext::write(stream, gpu_mem, host_mem, size, event);
}

bool GpuContext::readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    return GpuContext::read(stream, gpu_mem, host_mem, size, event);
}

bool GpuContext::flush(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("GpuContext::flush() Illegal stream argument.");
        return false;
    }
    return true;
}

bool GpuContext::finish(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("GpuContext::finish() Illegal stream argument.");
        return false;
    }
    return true;
}

// -------------------------
// GpuStream implementation.
// -------------------------

GpuStream::GpuStream(GpuContext const * c, GpuId i) : GpuIdWrapper(c)
{
    // EMPTY.
}

GpuStream::GpuStream(GpuStream const & obj) : GpuStream()
{
    (*this) = obj;
}

GpuStream::GpuStream(GpuStream && obj) : GpuStream()
{
    (*this) = std::move(obj);
}

GpuStream::~GpuStream()
{
    // EMPTY.
}

GpuStream & GpuStream::operator =(GpuStream const & obj)
{
    if (this != &obj) {
        assignIdWrapper(obj);
    }
    return *this;
}

GpuStream & GpuStream::operator =(GpuStream && obj)
{
    swap(obj);
    return *this;
}

void GpuStream::swap(GpuStream & obj)
{
    if (this != &obj) {
        swapIdWrapper(obj);
    }
}

// -----------------------------
// CpuEventGuard implementation.
// -----------------------------

CpuEventGuard::CpuEventGuard(GpuEvent * e) : event(e)
{
    if (event != nullptr) {
        event->start = nowNano();
    }
}

CpuEventGuard::~CpuEventGuard()
{
    if (event != nullptr) {
        event->stop = nowNano();
    }
}

GpuId CpuEventGuard::nowNano()
{
    using namespace std::chrono;
    return (GpuId)duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

// -----------------
// Global interface.
// -----------------

#ifndef _TBAG_SELECT_GPU_METHOD
#define _TBAG_SELECT_GPU_METHOD(gpu_type, default_return_value, method_name, param) \
    switch (gpu_type) {                                          \
    case GpuType::GT_CPU:     return    cpu::method_name(param); \
    case GpuType::GT_ACCEL:   return  accel::method_name(param); \
    case GpuType::GT_CUDA:    return   cuda::method_name(param); \
    case GpuType::GT_OPENCL:  return opencl::method_name(param); \
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;            \
    } return default_return_value;                               \
    /* -- END -- */
#endif

bool isSupport(GpuType type) TBAG_NOEXCEPT
{
    _TBAG_SELECT_GPU_METHOD(type, false, isSupport,);
}

int getPlatformCount(GpuType type)
{
    _TBAG_SELECT_GPU_METHOD(type, 0, getPlatformCount,);
}

GpuPlatforms getPlatformList(GpuType type)
{
    _TBAG_SELECT_GPU_METHOD(type, GpuPlatforms(), getPlatformList,);
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, GpuPlatformInfo(), getPlatformInfo, platform);
}

int getDeviceCount(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, 0, getDeviceCount, platform);
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, GpuDevices(), getDeviceList, platform);
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    _TBAG_SELECT_GPU_METHOD(device.TYPE, GpuDeviceInfo(), getDeviceInfo, device);
}

SharedGpuContext createContext(GpuDevice const & device)
{
    _TBAG_SELECT_GPU_METHOD(device.TYPE, SharedGpuContext(), createContext, device);
}

#if defined(_TBAG_SELECT_GPU_METHOD)
#undef _TBAG_SELECT_GPU_METHOD
#endif

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

