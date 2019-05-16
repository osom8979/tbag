/**
 * @file   Box.cpp
 * @brief  Box class implementation.
 * @author zer0
 * @date   2019-05-16
 */

#include <libtbag/box/Box.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using namespace libtbag::box::details;

Box::Box() : _data(std::make_shared<box_data>())
{
    assert(static_cast<bool>(_data));
    box_clear(_data.get());
    _data->opaque = nullptr;
}

Box::Box(std::nullptr_t) TBAG_NOEXCEPT : _data(nullptr)
{
    // EMPTY.
}

Box::Box(Box const & obj) TBAG_NOEXCEPT : Box(nullptr)
{
    (*this) = obj;
}

Box::Box(Box && obj) TBAG_NOEXCEPT : Box(nullptr)
{
    (*this) = std::move(obj);
}

Box::~Box()
{
    // EMPTY.
}

Box & Box::operator =(Box const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Box & Box::operator =(Box && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Box::copy(Box const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _data = obj._data;
    }
}

void Box::swap(Box & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _data.swap(obj._data);
    }
}

void Box::reset()
{
    _data.reset();
}

Err Box::resize_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = resize_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err Box::resize_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    return box_resize_vargs(_data.get(), type, device, ext, rank, ap);
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

