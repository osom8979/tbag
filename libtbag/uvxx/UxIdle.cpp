/**
 * @file   UxIdle.cpp
 * @brief  UxIdle class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxIdle.hpp>
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
 * UxIdle::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxIdle::Impl : private Noncopyable
{
private:
    UxIdle * _parent;

public:
    Impl(UxIdle * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxIdle implementation.
// ----------------------------

UxIdle::UxIdle() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxIdle::UxIdle(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxIdle::UxIdle(UxIdle const & obj) TBAG_NOEXCEPT : UxIdle(nullptr)
{
    (*this) = obj;
}

UxIdle::UxIdle(UxIdle && obj) TBAG_NOEXCEPT : UxIdle(nullptr)
{
    (*this) = std::move(obj);
}

UxIdle::~UxIdle()
{
    // EMPTY.
}

UxIdle & UxIdle::operator =(UxIdle const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxIdle & UxIdle::operator =(UxIdle && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxIdle::copy(UxIdle const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxIdle::swap(UxIdle & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxIdle::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

