/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
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

GpuContext::GpuContext(GpuDevice const & d, GpuId c) : GpuDevice(d), CONTEXT(c)
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
    return &stream.CONTEXT == this && stream.exists()
           && &gpu_mem.CONTEXT == this && gpu_mem.exists() && size <= gpu_mem.size
           && &host_mem.CONTEXT == this && host_mem.exists() && size <= host_mem.size;
}

GpuStream GpuContext::createStream() const
{
    return GpuStream(*this, 0);
}

bool GpuContext::releaseStream(GpuStream & stream) const
{
    if (&stream.CONTEXT != this || stream.exists()) {
        tDLogE("GpuContext::releaseStream() Illegal stream argument.");
        return false;
    }
    stream.stream = UNKNOWN_ID;
    return true;
}

GpuEvent GpuContext::createEvent(GpuStream const & stream) const
{
    if (&stream.CONTEXT != this || stream.exists()) {
        tDLogE("GpuContext::createEvent() Illegal stream argument.");
        return GpuEvent(*this);
    }
    return GpuEvent(*this, 0, 0);
}

bool GpuContext::syncEvent(GpuEvent const & event) const
{
    if (&event.CONTEXT != this || event.exists() == false) {
        tDLogE("GpuContext::syncEvent() Illegal event argument.");
        return false;
    }
    return true;
}

bool GpuContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (&event.CONTEXT != this || event.exists() == false) {
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
    if (&event.CONTEXT != this || event.exists() == false) {
        tDLogE("GpuContext::releaseEvent() Illegal event argument.");
        return false;
    }
    event.start = UNKNOWN_ID;
    event.stop  = UNKNOWN_ID;
    return true;
}

GpuProgram GpuContext::createProgram(std::string const &  source) const
{
    if (source.empty()) {
        tDLogE("GpuContext::createProgram() Illegal source.");
        return GpuProgram(*this);
    }
    return GpuProgram(*this, 0);
}

bool GpuContext::buildProgram(GpuProgram & program) const
{
    if (&program.CONTEXT != this || program.exists() == false) {
        tDLogE("GpuContext::buildProgram() Illegal program argument.");
        return false;
    }
    return true;
}

bool GpuContext::releaseProgram(GpuProgram & program) const
{
    if (&program.CONTEXT != this || program.exists() == false) {
        tDLogE("GpuContext::releaseProgram() Illegal program argument.");
        return false;
    }
    program.program = UNKNOWN_ID;
    return true;
}

GpuKernel GpuContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol) const
{
    if (&program.CONTEXT != this || program.exists() == false || kernel_symbol.empty()) {
        tDLogE("GpuContext::createKernel() Illegal arguments.");
        return GpuKernel(*this);
    }
    return GpuKernel(*this, 0);
}

bool GpuContext::releaseKernel(GpuKernel & kernel) const
{
    if (&kernel.CONTEXT != this || kernel.exists() == false) {
        tDLogE("GpuContext::releaseKernel() Illegal kernel context.");
        return false;
    }
    kernel.kernel = UNKNOWN_ID;
    return true;
}

GpuMemory GpuContext::malloc(std::size_t size) const
{
    GpuMemory result(*this);
    auto const  ALIGNED_SIZE = memory::getDefaultAlignedSize();
    auto const   PACKED_SIZE = algorithm::getPackedSize(size, ALIGNED_SIZE);
    auto const CAPACITY_SIZE = PACKED_SIZE * ALIGNED_SIZE;
    assert(CAPACITY_SIZE >= size);
    result.data     = memory::alignedMemoryAlloc(CAPACITY_SIZE, ALIGNED_SIZE);
    result.capacity = CAPACITY_SIZE;
    result.size     = size;
    tDLogIfD(isGpuVerbose(), "GpuContext::malloc({}) Aligned malloc MEM:{} CAP:{} SIZE:{}",
             size, result.data, result.capacity, result.size);
    return result;
}

bool GpuContext::free(GpuMemory & memory) const
{
    if (&memory.CONTEXT != this || memory.exists() == false) {
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

HostMemory GpuContext::mallocHost(std::size_t size, HostMemoryFlag flag) const
{
    if (HostMemoryFlag::HMF_DEFAULT != flag) {
        tDLogE("GpuContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return HostMemory(*this);
    }
    assert(flag == HostMemoryFlag::HMF_DEFAULT);
    return HostMemory(GpuContext::malloc(size));
}

bool GpuContext::freeHost(HostMemory & memory) const
{
    if (&memory.CONTEXT != this || memory.exists() == false) {
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
    if (&stream.CONTEXT != this || stream.exists() == false) {
        tDLogE("GpuContext::flush() Illegal stream argument.");
        return false;
    }
    return true;
}

bool GpuContext::finish(GpuStream & stream) const
{
    if (&stream.CONTEXT != this || stream.exists() == false) {
        tDLogE("GpuContext::finish() Illegal stream argument.");
        return false;
    }
    return true;
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

