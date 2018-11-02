/**
 * @file   Rect.cpp
 * @brief  Rect class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/graphics/Rect.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui  {
namespace core     {
namespace graphics {

/**
 * Rect::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct Rect::Impl : private libtbag::Noncopyable
{
private:
    Rect * _parent;

public:
    Impl(Rect * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// --------------------
// Rect implementation.
// --------------------

Rect::Rect() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Rect::Rect(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Rect::Rect(Rect const & obj) TBAG_NOEXCEPT : Rect(nullptr)
{
    (*this) = obj;
}

Rect::Rect(Rect && obj) TBAG_NOEXCEPT : Rect(nullptr)
{
    (*this) = std::move(obj);
}

Rect::~Rect()
{
    // EMPTY.
}

Rect & Rect::operator =(Rect const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Rect & Rect::operator =(Rect && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Rect::copy(Rect const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Rect::swap(Rect & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Rect::reset()
{
    _impl.reset();
}

} // namespace graphics
} // namespace core
} // namespace libtbui

