/**
 * @file   GpuMemory.cpp
 * @brief  GpuMemory class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuMemory.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// -----------------------------
// MemoryWrapper implementation.
// -----------------------------

MemoryWrapper::MemoryWrapper(GpuStream const & stream)
        : _stream(stream), _capacity(0), _size(0), _data(nullptr)
{
    // EMPTY.
}

MemoryWrapper::~MemoryWrapper()
{
    // EMPTY.
}

GpuContext const & MemoryWrapper::atContext() const TBAG_NOEXCEPT
{
    return _stream.atContext();
}

bool MemoryWrapper::isSameContext(GpuContext const & context) const TBAG_NOEXCEPT
{
    return _stream.isSameContext(context);
}

// -------------------------
// GpuMemory implementation.
// -------------------------

GpuMemory::GpuMemory(GpuStream const & stream) : MemoryWrapper(stream)
{
    // EMPTY.
}

GpuMemory::~GpuMemory()
{
    // EMPTY.
}

Err GpuMemory::alloc(std::size_t size)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return atContext().malloc(*this, size);
}

Err GpuMemory::free()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().free(*this);
}

Err GpuMemory::copy(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().copy(atStream(), *this, memory, size, event);
}

Err GpuMemory::copy(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().read(atStream(), *this, memory, size, event);
}

Err GpuMemory::copyAsync(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().copyAsync(atStream(), *this, memory, size, event);
}

Err GpuMemory::copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().readAsync(atStream(), *this, memory, size, event);
}

// --------------------------
// HostMemory implementation.
// --------------------------

HostMemory::HostMemory(GpuStream const & stream)
        : MemoryWrapper(stream), _flag(HostMemoryFlag::HMF_DEFAULT)
{
    // EMPTY.
}

HostMemory::~HostMemory()
{
    // EMPTY.
}

Err HostMemory::alloc(std::size_t size, HostMemoryFlag flag)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return atContext().mallocHost(*this, size, flag);
}

Err HostMemory::free()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().freeHost(*this);
}

Err HostMemory::copy(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().write(atStream(), memory, *this, size, event);
}

Err HostMemory::copy(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().copy(atStream(), *this, memory, size, event);
}

Err HostMemory::copyAsync(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().writeAsync(atStream(), memory, *this, size, event);
}

Err HostMemory::copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return atContext().copyAsync(atStream(), *this, memory, size, event);
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

