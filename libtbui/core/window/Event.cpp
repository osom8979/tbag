/**
 * @file   Event.cpp
 * @brief  Event class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbui/core/window/Event.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

namespace libtbui {
namespace core    {
namespace window  {

/**
 * Event::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-03
 */
struct Event::Impl : private libtbag::Noncopyable
{
private:
    Event * _parent;

public:
    Impl(Event * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ---------------------
// Event implementation.
// ---------------------

Event::Event() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Event::Event(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Event::Event(Event const & obj) TBAG_NOEXCEPT : Event(nullptr)
{
    (*this) = obj;
}

Event::Event(Event && obj) TBAG_NOEXCEPT : Event(nullptr)
{
    (*this) = std::move(obj);
}

Event::~Event()
{
    // EMPTY.
}

Event & Event::operator =(Event const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Event & Event::operator =(Event && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Event::copy(Event const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Event::swap(Event & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Event::reset()
{
    _impl.reset();
}

} // namespace window
} // namespace core
} // namespace libtbui

