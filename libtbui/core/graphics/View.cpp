/**
 * @file   View.cpp
 * @brief  View class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/graphics/View.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui  {
namespace core     {
namespace graphics {

/**
 * View::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct View::Impl : private libtbag::Noncopyable
{
private:
    View * _parent;

public:
    Impl(View * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// --------------------
// View implementation.
// --------------------

View::View() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

View::View(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

View::View(View const & obj) TBAG_NOEXCEPT : View(nullptr)
{
    (*this) = obj;
}

View::View(View && obj) TBAG_NOEXCEPT : View(nullptr)
{
    (*this) = std::move(obj);
}

View::~View()
{
    // EMPTY.
}

View & View::operator =(View const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

View & View::operator =(View && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void View::copy(View const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void View::swap(View & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void View::reset()
{
    _impl.reset();
}

} // namespace graphics
} // namespace core
} // namespace libtbui

