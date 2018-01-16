/**
 * @file   MemoryWrapper.cpp
 * @brief  MemoryWrapper class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/MemoryWrapper.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

MemoryWrapper::MemoryWrapper(GpuContext const * c, GpuStream const * s)
        : _context(c), _stream(s), _capacity(0), _size(0), _data(nullptr)
{
    // EMPTY.
}

MemoryWrapper::MemoryWrapper(MemoryWrapper const & obj) : MemoryWrapper()
{
    (*this) = obj;
}

MemoryWrapper::MemoryWrapper(MemoryWrapper && obj) : MemoryWrapper()
{
    (*this) = std::move(obj);
}

MemoryWrapper::~MemoryWrapper()
{
    // EMPTY.
}

MemoryWrapper & MemoryWrapper::operator =(MemoryWrapper const & obj)
{
    if (this != &obj) {
        _context  = obj._context;
        _stream   = obj._stream;
        _capacity = obj._capacity;
        _size     = obj._size;
        _data     = obj._data;
    }
    return *this;
}

MemoryWrapper & MemoryWrapper::operator =(MemoryWrapper && obj)
{
    swap(obj);
    return *this;
}

void MemoryWrapper::swap(MemoryWrapper & obj)
{
    if (this != &obj) {
        std::swap(_context , obj._context);
        std::swap(_stream  , obj._stream);
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

