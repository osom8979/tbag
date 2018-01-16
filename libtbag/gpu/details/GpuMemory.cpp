/**
 * @file   GpuMemory.cpp
 * @brief  GpuMemory class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuMemory.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

GpuMemory::GpuMemory(GpuContext const * c)
        : _context(c), _capacity(0), _size(0), _data(nullptr)
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

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

