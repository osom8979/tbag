/**
 * @file   HostMemory.cpp
 * @brief  HostMemory class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/HostMemory.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

HostMemory::HostMemory(GpuContext const * c, GpuStream const * s)
        : MemoryWrapper(c, s), _flag(HostMemoryFlag::HMF_DEFAULT)
{
    // EMPTY.
}

HostMemory::HostMemory(MemoryWrapper const & mem)
        : MemoryWrapper(mem), _flag(HostMemoryFlag::HMF_DEFAULT)
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
        MemoryWrapper::operator=(obj);
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
        MemoryWrapper::swap(obj);
        std::swap(_flag, obj._flag);
    }
}

Err HostMemory::alloc(std::size_t size, HostMemoryFlag flag)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return (_context != nullptr ? _context->mallocHost(*this, size, flag) : Err::E_NULLPTR);
}

Err HostMemory::free()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->freeHost(*this) : Err::E_NULLPTR);
}

Err HostMemory::copy(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    if (_context == nullptr || _stream == nullptr) {
        return Err::E_NULLPTR;
    }
    return _context->write(*_stream, memory, *this, size, event);
}

Err HostMemory::copy(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    if (_context == nullptr || _stream == nullptr) {
        return Err::E_NULLPTR;
    }
    return _context->copy(*_stream, *this, memory, size, event);
}

Err HostMemory::copyAsync(GpuMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    if (_context == nullptr || _stream == nullptr) {
        return Err::E_NULLPTR;
    }
    return _context->writeAsync(*_stream, memory, *this, size, event);
}

Err HostMemory::copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event) const
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    if (_context == nullptr || _stream == nullptr) {
        return Err::E_NULLPTR;
    }
    return _context->copyAsync(*_stream, *this, memory, size, event);
}

HostMemory HostMemory::instance(GpuContext const * c, std::size_t size, HostMemoryFlag flag)
{
    if (c == nullptr) {
        return HostMemory();
    }

    HostMemory memory(c);
    if (isSuccess(memory.alloc(size, flag))) {
        return memory;
    }
    return HostMemory();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

