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

bool GpuContext::validateMemory(GpuStream const & stream, GpuMemory const & gpu_mem,
                                HostMemory const & host_mem, std::size_t size) const TBAG_NOEXCEPT
{
    return stream.validate(this)
           && gpu_mem.validate(this) && size <= gpu_mem.size()
           && host_mem.validate(this) && size <= host_mem.size();
}

bool GpuContext::createStream(GpuStream & stream) const
{
    if (stream.isSameContext(this) == false) {
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
    if (event.isSameContext(this) == false) {
        tDLogE("GpuContext::createEvent() Illegal event argument.");
        return false;
    }
    event.setStart(0);
    event.setStop(0);
    return true;
}

bool GpuContext::syncEvent(GpuEvent const & event) const
{
    if (event.validate(this) == false) {
        tDLogE("GpuContext::syncEvent() Illegal event argument.");
        return false;
    }
    return true;
}

bool GpuContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.validate(this) == false) {
        tDLogE("GpuContext::elapsedEvent() Illegal event argument.");
        return false;
    }
    if (millisec != nullptr) {
        *millisec = (event.getStop() - event.getStart()) * 1.0e-6f;
    }
    return true;
}

bool GpuContext::releaseEvent(GpuEvent & event) const
{
    if (event.validate(this) == false) {
        tDLogE("GpuContext::releaseEvent() Illegal event argument.");
        return false;
    }
    event.clearIds();
    return true;
}

bool GpuContext::createProgram(std::string const & source, GpuProgram & program) const
{
    if (source.empty()) {
        tDLogE("GpuContext::createProgram() Empty source.");
        return false;
    }
    if (program.isSameContext(this) == false) {
        tDLogE("GpuContext::createProgram() Illegal program argument.");
        return false;
    }

    program.setId(0);
    return true;
}

bool GpuContext::buildProgram(GpuProgram & program) const
{
    if (program.validate(this) == false) {
        tDLogE("GpuContext::buildProgram() Illegal program argument.");
        return false;
    }
    return true;
}

bool GpuContext::releaseProgram(GpuProgram & program) const
{
    if (program.validate(this) == false) {
        tDLogE("GpuContext::releaseProgram() Illegal program argument.");
        return false;
    }
    program.clearId();
    return true;
}

bool GpuContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const
{
    if (program.validate(this) == false) {
        tDLogE("GpuContext::createKernel() Illegal program arguments.");
        return false;
    }
    if (kernel_symbol.empty()) {
        tDLogE("GpuContext::createKernel() Empty kernel symbol.");
        return false;
    }
    if (kernel.isSameContext(this) == false) {
        tDLogE("GpuContext::createKernel() Illegal kernel arguments.");
        return false;
    }

    kernel.setId(0);
    return true;
}

bool GpuContext::releaseKernel(GpuKernel & kernel) const
{
    if (kernel.validate(this) == false) {
        tDLogE("GpuContext::releaseKernel() Illegal kernel context.");
        return false;
    }
    kernel.clearId();
    return true;
}

bool GpuContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.isSameContext(this) == false) {
        tDLogE("GpuContext::malloc() Illegal memory arguments.");
        return false;
    }

    auto const  ALIGNED_SIZE = memory::getDefaultAlignedSize();
    auto const   PACKED_SIZE = algorithm::getPackedSize(size, ALIGNED_SIZE);
    auto const CAPACITY_SIZE = PACKED_SIZE * ALIGNED_SIZE;
    assert(CAPACITY_SIZE >= size);

    void * data = memory::alignedMemoryAlloc(CAPACITY_SIZE, ALIGNED_SIZE);
    memory.set(data, CAPACITY_SIZE, size);

    tDLogIfD(isGpuVerbose(), "GpuContext::malloc({}) Aligned malloc MEM:{} CAP:{} SIZE:{}",
             size, memory.data(), memory.capacity(), memory.size());
    return true;
}

bool GpuContext::free(GpuMemory & memory) const
{
    if (memory.validate(this) == false) {
        tDLogE("GpuContext::free() Illegal memory argument");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "GpuContext::free() Aligned free MEM:{} CAP:{} SIZE:{}",
             memory.data(), memory.capacity(), memory.size());
    memory::alignedMemoryFree(memory.data());
    memory.clear();
    return true;
}

bool GpuContext::mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag) const
{
    if (memory.isSameContext(this) == false) {
        tDLogE("GpuContext::mallocHost() Illegal memory argument");
        return false;
    }
    if (HostMemoryFlag::HMF_DEFAULT != flag) {
        tDLogE("GpuContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return false;
    }

    auto const  ALIGNED_SIZE = memory::getDefaultAlignedSize();
    auto const   PACKED_SIZE = algorithm::getPackedSize(size, ALIGNED_SIZE);
    auto const CAPACITY_SIZE = PACKED_SIZE * ALIGNED_SIZE;

    assert(flag == HostMemoryFlag::HMF_DEFAULT);
    assert(CAPACITY_SIZE >= size);

    void * data = memory::alignedMemoryAlloc(CAPACITY_SIZE, ALIGNED_SIZE);
    memory.set(data, CAPACITY_SIZE, size);
    memory.setFlag(flag);

    tDLogIfD(isGpuVerbose(), "GpuContext::mallocHost({}) Aligned malloc MEM:{} CAP:{} SIZE:{}",
             size, memory.data(), memory.capacity(), memory.size());
    return true;
}

bool GpuContext::freeHost(HostMemory & memory) const
{
    if (memory.validate(this) == false) {
        tDLogE("GpuContext::freeHost() Illegal memory argument");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "GpuContext::freeHost() Aligned free MEM:{} CAP:{} SIZE:{}",
             memory.data(), memory.capacity(), memory.size());
    memory::alignedMemoryFree(memory.data());
    memory.clear();
    return true;
}

bool GpuContext::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("GpuContext::write() Illegal arguments.");
        return false;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(gpu_mem.data(), host_mem.data(), size);
    return true;
}

bool GpuContext::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("GpuContext::read() Illegal arguments.");
        return false;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(host_mem.data(), gpu_mem.data(), size);
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

// ----------------------------
// GpuIdWrapper implementation.
// ----------------------------

GpuIdWrapper::GpuIdWrapper(GpuContext const * c, GpuId i) : _context(c), _id(i)
{
    // EMPTY.
}

GpuIdWrapper::GpuIdWrapper(GpuIdWrapper const & obj) : GpuIdWrapper()
{
    (*this) = obj;
}

GpuIdWrapper::GpuIdWrapper(GpuIdWrapper && obj) : GpuIdWrapper()
{
    (*this) = std::move(obj);
}

GpuIdWrapper::~GpuIdWrapper()
{
    // EMPTY.
}

GpuIdWrapper & GpuIdWrapper::operator =(GpuIdWrapper const & obj)
{
    assign(obj);
    return *this;
}

GpuIdWrapper & GpuIdWrapper::operator =(GpuIdWrapper && obj)
{
    swap(obj);
    return *this;
}

void GpuIdWrapper::swap(GpuIdWrapper & obj)
{
    if (this != &obj) {
        std::swap(_context, obj._context);
        std::swap(_id, obj._id);
    }
}

void GpuIdWrapper::assign(GpuIdWrapper const & obj)
{
    if (this != &obj) {
        _context = obj._context;
        _id = obj._id;
    }
}

// -------------------------
// GpuStream implementation.
// -------------------------

GpuStream::GpuStream(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
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
    release();
}

GpuStream & GpuStream::operator =(GpuStream const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
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
        GpuIdWrapper::swap(obj);
    }
}

bool GpuStream::create()
{
    return (_context != nullptr ? _context->createStream(*this) : false);
}

bool GpuStream::release()
{
    return (_context != nullptr ? _context->releaseStream(*this) : false);
}

GpuStream GpuStream::create(GpuContext const * c)
{
    GpuStream stream(c);
    if (stream.create()) {
        return stream;
    }
    return GpuStream(nullptr);
}

// ------------------------
// GpuEvent implementation.
// ------------------------

GpuEvent::GpuEvent(GpuContext const * c, GpuStream const * s, GpuId start, GpuId stop)
        : _context(c), _stream(s), _start(start), _stop(stop)
{
    // EMPTY.
}

GpuEvent::GpuEvent(GpuEvent const & obj) : GpuEvent()
{
    (*this) = obj;
}

GpuEvent::GpuEvent(GpuEvent && obj) : GpuEvent()
{
    (*this) = std::move(obj);
}

GpuEvent::~GpuEvent()
{
    // EMPTY.
}

GpuEvent & GpuEvent::operator =(GpuEvent const & obj)
{
    if (this != &obj) {
        _context = obj._context;
        _stream  = obj._stream;
        _start   = obj._start;
        _stop    = obj._stop;
    }
    return *this;
}

GpuEvent & GpuEvent::operator =(GpuEvent && obj)
{
    swap(obj);
    return *this;
}

void GpuEvent::swap(GpuEvent & obj)
{
    if (this != &obj) {
        std::swap(_context, obj._context);
        std::swap(_stream, obj._stream);
        std::swap(_start, obj._start);
        std::swap(_stop, obj._stop);
    }
}

// --------------------------
// GpuProgram implementation.
// --------------------------

GpuProgram::GpuProgram(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
{
    // EMPTY.
}

GpuProgram::GpuProgram(GpuProgram const & obj) : GpuProgram()
{
    (*this) = obj;
}

GpuProgram::GpuProgram(GpuProgram && obj) : GpuProgram()
{
    (*this) = std::move(obj);
}

GpuProgram::~GpuProgram()
{
    // EMPTY.
}

GpuProgram & GpuProgram::operator =(GpuProgram const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
    }
    return *this;
}

GpuProgram & GpuProgram::operator =(GpuProgram && obj)
{
    swap(obj);
    return *this;
}

void GpuProgram::swap(GpuProgram & obj)
{
    if (this != &obj) {
        GpuIdWrapper::swap(obj);
    }
}

// -------------------------
// GpuKernel implementation.
// -------------------------

GpuKernel::GpuKernel(GpuContext const * c, GpuId i) : GpuIdWrapper(c, i)
{
    // EMPTY.
}

GpuKernel::GpuKernel(GpuKernel const & obj) : GpuKernel()
{
    (*this) = obj;
}

GpuKernel::GpuKernel(GpuKernel && obj) : GpuKernel()
{
    (*this) = std::move(obj);
}

GpuKernel::~GpuKernel()
{
    // EMPTY.
}

GpuKernel & GpuKernel::operator =(GpuKernel const & obj)
{
    if (this != &obj) {
        GpuIdWrapper::assign(obj);
    }
    return *this;
}

GpuKernel & GpuKernel::operator =(GpuKernel && obj)
{
    swap(obj);
    return *this;
}

void GpuKernel::swap(GpuKernel & obj)
{
    if (this != &obj) {
        GpuIdWrapper::swap(obj);
    }
}

// -------------------------
// GpuMemory implementation.
// -------------------------

GpuMemory::GpuMemory(GpuContext const * c) : _context(c), _capacity(0), _size(0), _data(nullptr)
{
    // EMPTY.
}

GpuMemory::GpuMemory(GpuMemory const & obj) : GpuMemory()
{
    (*this) = obj;
}

GpuMemory::GpuMemory(GpuMemory && obj) : GpuMemory()
{
    (*this) = std::move(obj);
}

GpuMemory::~GpuMemory()
{
    // EMPTY.
}

GpuMemory & GpuMemory::operator =(GpuMemory const & obj)
{
    if (this != &obj) {
        _context  = obj._context;
        _capacity = obj._capacity;
        _size     = obj._size;
        _data     = obj._data;
    }
    return *this;
}

GpuMemory & GpuMemory::operator =(GpuMemory && obj)
{
    swap(obj);
    return *this;
}

void GpuMemory::swap(GpuMemory & obj)
{
    if (this != &obj) {
        std::swap(_context , obj._context);
        std::swap(_capacity, obj._capacity);
        std::swap(_size    , obj._size);
        std::swap(_data    , obj._data);
    }
}


// -------------------------
// HostMemory implementation.
// -------------------------

HostMemory::HostMemory(GpuContext const * c) : GpuMemory(c), _flag(HostMemoryFlag::HMF_DEFAULT)
{
    // EMPTY.
}

HostMemory::HostMemory(HostMemory const & obj) : HostMemory()
{
    (*this) = obj;
}

HostMemory::HostMemory(HostMemory && obj) : HostMemory()
{
    (*this) = std::move(obj);
}

HostMemory::~HostMemory()
{
    // EMPTY.
}

HostMemory & HostMemory::operator =(HostMemory const & obj)
{
    if (this != &obj) {
        GpuMemory::operator=(obj);
        _flag = obj._flag;
    }
    return *this;
}

HostMemory & HostMemory::operator =(HostMemory && obj)
{
    HostMemory::swap(obj);
    return *this;
}

void HostMemory::swap(HostMemory & obj)
{
    if (this != &obj) {
        GpuMemory::swap(obj);
        std::swap(_flag, obj._flag);
    }
}

// -----------------------------
// CpuEventGuard implementation.
// -----------------------------

CpuEventGuard::CpuEventGuard(GpuEvent * e) : _event(e)
{
    if (_event != nullptr) {
        _event->setStart(nowNano());
    }
}

CpuEventGuard::~CpuEventGuard()
{
    if (_event != nullptr) {
        _event->setStop(nowNano());
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

