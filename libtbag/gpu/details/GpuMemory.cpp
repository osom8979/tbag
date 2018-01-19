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

GpuMemory::GpuMemory(GpuStream const & stream, std::size_t size) : MemoryWrapper(stream)
{
    if (isFailure(atContext().malloc(*this, size))) {
        throw std::bad_alloc();
    }
}

GpuMemory::~GpuMemory()
{
    if (validate()) {
        atContext().free(*this);
    }
}

void GpuMemory::set(void * data, std::size_t capacity, std::size_t size) TBAG_NOEXCEPT
{
    _data     = data;
    _capacity = capacity;
    _size     = size;
}

void GpuMemory::clear() TBAG_NOEXCEPT
{
    _data     = nullptr;
    _capacity = 0;
    _size     = 0;
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

HostMemory::HostMemory(GpuStream const & stream, std::size_t size, HostMemoryFlag flag)
        : MemoryWrapper(stream), _flag(flag)
{
    if (isFailure(atContext().mallocHost(*this, size, flag))) {
        throw std::bad_alloc();
    }
}

HostMemory::~HostMemory()
{
    if (validate()) {
        atContext().freeHost(*this);
    }
}

void HostMemory::set(void * data, std::size_t capacity, std::size_t size, HostMemoryFlag flag) TBAG_NOEXCEPT
{
    _data     = data;
    _capacity = capacity;
    _size     = size;
    _flag     = flag;
}

void HostMemory::clear() TBAG_NOEXCEPT
{
    _data     = nullptr;
    _capacity = 0;
    _size     = 0;
    _flag     = HostMemoryFlag::HMF_DEFAULT;
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

