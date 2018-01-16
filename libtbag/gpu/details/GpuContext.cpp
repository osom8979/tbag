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

Err GpuContext::createStream(GpuStream & stream) const
{
    if (stream.isSameContext(this) == false) {
        return Err::E_ILLARGS;
    }
    stream.setId(0);
    return Err::E_SUCCESS;
}

Err GpuContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    stream.clearId();
    return Err::E_SUCCESS;
}

Err GpuContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(this) == false || event.isSameContext(this) == false) {
        return Err::E_ILLARGS;
    }
    event.setStart(0);
    event.setStop(0);
    return Err::E_SUCCESS;
}

Err GpuContext::syncEvent(GpuEvent const & event) const
{
    if (event.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

Err GpuContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    if (millisec != nullptr) {
        *millisec = (event.getStop() - event.getStart()) * 1.0e-6f;
    }
    return Err::E_SUCCESS;
}

Err GpuContext::releaseEvent(GpuEvent & event) const
{
    if (event.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    event.clearIds();
    return Err::E_SUCCESS;
}

Err GpuContext::createProgram(std::string const & source, GpuProgram & program) const
{
    if (source.empty() || program.isSameContext(this) == false) {
        return Err::E_ILLARGS;
    }
    program.setId(0);
    return Err::E_SUCCESS;
}

Err GpuContext::buildProgram(GpuProgram & program) const
{
    if (program.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

Err GpuContext::releaseProgram(GpuProgram & program) const
{
    if (program.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    program.clearId();
    return Err::E_SUCCESS;
}

Err GpuContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const
{
    if (program.validate(this) == false || kernel_symbol.empty() || kernel.isSameContext(this) == false) {
        return Err::E_ILLARGS;
    }
    kernel.setId(0);
    return Err::E_SUCCESS;
}

Err GpuContext::releaseKernel(GpuKernel & kernel) const
{
    if (kernel.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    kernel.clearId();
    return Err::E_SUCCESS;
}

Err GpuContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.isSameContext(this) == false) {
        return Err::E_ILLARGS;
    }

    auto const  ALIGNED_SIZE = memory::getDefaultAlignedSize();
    auto const   PACKED_SIZE = algorithm::getPackedSize(size, ALIGNED_SIZE);
    auto const CAPACITY_SIZE = PACKED_SIZE * ALIGNED_SIZE;
    assert(CAPACITY_SIZE >= size);

    void * data = memory::alignedMemoryAlloc(CAPACITY_SIZE, ALIGNED_SIZE);
    memory.set(data, CAPACITY_SIZE, size);

    tDLogIfD(isGpuVerbose(), "GpuContext::malloc({}) Aligned malloc MEM:{} CAP:{} SIZE:{}",
             size, memory.data(), memory.capacity(), memory.size());
    return Err::E_SUCCESS;
}

Err GpuContext::free(GpuMemory & memory) const
{
    if (memory.validate(this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "GpuContext::free() Aligned free MEM:{} CAP:{} SIZE:{}",
             memory.data(), memory.capacity(), memory.size());
    memory::alignedMemoryFree(memory.data());
    memory.clear();
    return Err::E_SUCCESS;
}

Err GpuContext::mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag) const
{
    if (HostMemoryFlag::HMF_DEFAULT != flag) {
        tDLogE("GpuContext::mallocHost() Unsupported flag: {}", static_cast<int>(flag));
        return Err::E_ILLARGS;
    }
    assert(flag == HostMemoryFlag::HMF_DEFAULT);
    return GpuContext::malloc(memory, size);
}

Err GpuContext::freeHost(HostMemory & memory) const
{
    return GpuContext::free(memory);
}

Err GpuContext::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(gpu_mem.data(), host_mem.data(), size);
    return Err::E_SUCCESS;
}

Err GpuContext::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    CpuEventGuard const EVENT_LOCK(event);
    ::memcpy(host_mem.data(), gpu_mem.data(), size);
    return Err::E_SUCCESS;
}

Err GpuContext::writeAsync(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    return GpuContext::write(stream, gpu_mem, host_mem, size, event);
}

Err GpuContext::readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    return GpuContext::read(stream, gpu_mem, host_mem, size, event);
}

Err GpuContext::flush(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

Err GpuContext::finish(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

