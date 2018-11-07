/**
 * @file   UxFsEvent.cpp
 * @brief  UxFsEvent class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxFsEvent.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

/**
 * UxFsEvent::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxFsEvent::Impl : private Noncopyable
{
private:
    UxFsEvent * _parent;

public:
    Impl(UxFsEvent * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxFsEvent implementation.
// ----------------------------

UxFsEvent::UxFsEvent() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxFsEvent::UxFsEvent(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxFsEvent::UxFsEvent(UxFsEvent const & obj) TBAG_NOEXCEPT : UxFsEvent(nullptr)
{
    (*this) = obj;
}

UxFsEvent::UxFsEvent(UxFsEvent && obj) TBAG_NOEXCEPT : UxFsEvent(nullptr)
{
    (*this) = std::move(obj);
}

UxFsEvent::~UxFsEvent()
{
    // EMPTY.
}

UxFsEvent & UxFsEvent::operator =(UxFsEvent const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxFsEvent & UxFsEvent::operator =(UxFsEvent && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxFsEvent::copy(UxFsEvent const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxFsEvent::swap(UxFsEvent & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxFsEvent::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

