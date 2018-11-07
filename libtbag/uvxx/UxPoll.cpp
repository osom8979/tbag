/**
 * @file   UxPoll.cpp
 * @brief  UxPoll class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxPoll.hpp>
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
 * UxPoll::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxPoll::Impl : private Noncopyable
{
private:
    UxPoll * _parent;

public:
    Impl(UxPoll * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxPoll implementation.
// ----------------------------

UxPoll::UxPoll() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxPoll::UxPoll(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxPoll::UxPoll(UxPoll const & obj) TBAG_NOEXCEPT : UxPoll(nullptr)
{
    (*this) = obj;
}

UxPoll::UxPoll(UxPoll && obj) TBAG_NOEXCEPT : UxPoll(nullptr)
{
    (*this) = std::move(obj);
}

UxPoll::~UxPoll()
{
    // EMPTY.
}

UxPoll & UxPoll::operator =(UxPoll const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPoll & UxPoll::operator =(UxPoll && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPoll::copy(UxPoll const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxPoll::swap(UxPoll & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxPoll::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

