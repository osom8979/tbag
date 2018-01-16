/**
 * @file   GpuMemory.cpp
 * @brief  GpuMemory class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuMemory.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuMemory::GpuMemory(GpuContext const * c) : MemoryWrapper(c)
{
    // EMPTY.
}

GpuMemory::GpuMemory(MemoryWrapper const & mem) : MemoryWrapper(mem)
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
        MemoryWrapper::operator=(obj);
    }
    return *this;
}

GpuMemory & GpuMemory::operator =(GpuMemory && obj)
{
    GpuMemory::swap(obj);
    return *this;
}

void GpuMemory::swap(GpuMemory & obj)
{
    if (this != &obj) {
        MemoryWrapper::swap(obj);
    }
}

Err GpuMemory::alloc(std::size_t size)
{
    if (validate()) {
        return Err::E_ALREADY;
    }
    return (_context != nullptr ? _context->malloc(*this, size) : Err::E_NULLPTR);
}

Err GpuMemory::free()
{
    if (validate() == false) {
        return Err::E_ILLSTATE;
    }
    return (_context != nullptr ? _context->free(*this) : Err::E_NULLPTR);
}

GpuMemory GpuMemory::instance(GpuContext const * c, std::size_t size)
{
    if (c == nullptr) {
        return GpuMemory();
    }

    GpuMemory memory(c);
    if (isSuccess(memory.alloc(size))) {
        return memory;
    }
    return GpuMemory();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

