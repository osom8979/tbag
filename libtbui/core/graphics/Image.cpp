/**
 * @file   Image.cpp
 * @brief  Image class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/graphics/Image.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui  {
namespace core     {
namespace graphics {

/**
 * Image::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct Image::Impl : private libtbag::Noncopyable
{
private:
    Image * _parent;

public:
    Impl(Image * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ---------------------
// Image implementation.
// ---------------------

Image::Image() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Image::Image(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Image::Image(Image const & obj) TBAG_NOEXCEPT : Image(nullptr)
{
    (*this) = obj;
}

Image::Image(Image && obj) TBAG_NOEXCEPT : Image(nullptr)
{
    (*this) = std::move(obj);
}

Image::~Image()
{
    // EMPTY.
}

Image & Image::operator =(Image const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Image & Image::operator =(Image && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Image::copy(Image const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Image::swap(Image & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Image::reset()
{
    _impl.reset();
}

} // namespace graphics
} // namespace core
} // namespace libtbui

