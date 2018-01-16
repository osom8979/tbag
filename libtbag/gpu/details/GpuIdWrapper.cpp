/**
 * @file   GpuIdWrapper.cpp
 * @brief  GpuIdWrapper class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/details/GpuIdWrapper.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

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

void GpuIdWrapper::assign(GpuIdWrapper const & obj)
{
    if (this != &obj) {
        _context = obj._context;
        _id      = obj._id;
    }
}

void GpuIdWrapper::swap(GpuIdWrapper & obj)
{
    if (this != &obj) {
        std::swap(_context, obj._context);
        std::swap(_id     , obj._id);
    }
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

