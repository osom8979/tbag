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
#include <cstdlib>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using namespace libtbag::box::details;

Box::Box() : _data(nullptr)
{
    auto * box = (box_data*)::malloc(sizeof(box_data));
    assert(box != nullptr);
    box_clear(box);
    box->opaque = nullptr;

    _data.reset(box, [](box_data * ptr){
        box_free(ptr);
        ::free(ptr);
    });
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

Err Box::reshape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = reshape_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err Box::reshape_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = reshape_vargs(type, rank, ap);
    va_end(ap);
    return CODE;
}

Err Box::reshape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    return box_resize_vargs(_data.get(), type, device, ext, rank, ap);
}

Err Box::reshape_vargs(btype type, ui32 rank, va_list ap)
{
    btype reshape_device = device_cpu();
    ui64 const * reshape_ext = nullptr;
    if (exists()) {
        if (!is_device_none()) {
            reshape_device = device();
        }
        reshape_ext = ext();
    }
    return reshape_vargs(type, reshape_device, reshape_ext, rank, ap);
}

Box Box::shape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto result = shape_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return result;
}

Box Box::shape_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto result = shape_vargs(type, rank, ap);
    va_end(ap);
    return result;
}

Box Box::shape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    Box result;
    assert(result);
    auto const CODE = result.reshape_vargs(type, device, ext, rank, ap);
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::shape_vargs(btype type, ui32 rank, va_list ap)
{
    Box result;
    assert(result);
    auto const CODE = result.reshape_vargs(type, rank, ap);
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Err Box::copyFromData(Box const & box)
{
    return box_data_copy(get(), box.get());
}

Err Box::copyToData(Box & box) const
{
    return box.copyFromData(*this);
}

Err Box::copyFromInfo(Box const & box)
{
    return box_info_checked_copy(get(), box.get()) ? E_SUCCESS : E_ECOPY;
}

Err Box::copyToInfo(Box & box) const
{
    return box.copyFromInfo(*this);
}

Box Box::clone() const
{
    Box result;
    auto const CODE = box_clone(result.get(), get());
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::asType(btype type) const
{
    Box result;
    auto code = box_malloc_copy_dims(result.get(), type, device(), ext(), dims(), dims_capacity(), rank());
    if (isFailure(code)) {
        return Box(nullptr);
    }
    code = result.copyFromData(*this);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    code = result.copyFromInfo(*this);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
