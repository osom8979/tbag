/**
 * @file   Window.cpp
 * @brief  Window class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/window/Window.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui {
namespace core    {
namespace window  {

/**
 * Window::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct Window::Impl : private libtbag::Noncopyable
{
private:
    Window * _parent;

public:
    Impl(Window * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------
// Window implementation.
// ----------------------

Window::Window() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Window::Window(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Window::Window(Window const & obj) TBAG_NOEXCEPT : Window(nullptr)
{
    (*this) = obj;
}

Window::Window(Window && obj) TBAG_NOEXCEPT : Window(nullptr)
{
    (*this) = std::move(obj);
}

Window::~Window()
{
    // EMPTY.
}

Window & Window::operator =(Window const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Window & Window::operator =(Window && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Window::copy(Window const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Window::swap(Window & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Window::reset()
{
    _impl.reset();
}

} // namespace window
} // namespace core
} // namespace libtbui

