/**
 * @file   GpuContext.cpp
 * @brief  GpuContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuEvent.hpp>
#include <libtbag/gpu/details/GpuIdWrapper.hpp>
#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/gpu/details/GpuMemory.hpp>
#include <libtbag/gpu/details/GpuProgram.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/gpu/details/HostMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/memory/AlignedMemory.hpp>
#include <libtbag/algorithm/Pack.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuContext::GpuContext(GpuDevice const & d, GpuId c) : GpuDevice(d), CONTEXT(c)
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

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

