/**
 * @file   UxTimer.cpp
 * @brief  UxTimer class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxTimer.hpp>
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
 * UxTimer::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxTimer::Impl : private Noncopyable
{
private:
    UxTimer * _parent;

public:
    Impl(UxTimer * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxTimer implementation.
// ----------------------------

UxTimer::UxTimer() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxTimer::UxTimer(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxTimer::UxTimer(UxTimer const & obj) TBAG_NOEXCEPT : UxTimer(nullptr)
{
    (*this) = obj;
}

UxTimer::UxTimer(UxTimer && obj) TBAG_NOEXCEPT : UxTimer(nullptr)
{
    (*this) = std::move(obj);
}

UxTimer::~UxTimer()
{
    // EMPTY.
}

UxTimer & UxTimer::operator =(UxTimer const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTimer & UxTimer::operator =(UxTimer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTimer::copy(UxTimer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxTimer::swap(UxTimer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxTimer::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

