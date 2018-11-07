/**
 * @file   UxAsync.cpp
 * @brief  UxAsync class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxAsync.hpp>
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
 * UxAsync::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxAsync::Impl : private Noncopyable
{
private:
    UxAsync * _parent;

public:
    Impl(UxAsync * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxAsync implementation.
// ----------------------------

UxAsync::UxAsync() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxAsync::UxAsync(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxAsync::UxAsync(UxAsync const & obj) TBAG_NOEXCEPT : UxAsync(nullptr)
{
    (*this) = obj;
}

UxAsync::UxAsync(UxAsync && obj) TBAG_NOEXCEPT : UxAsync(nullptr)
{
    (*this) = std::move(obj);
}

UxAsync::~UxAsync()
{
    // EMPTY.
}

UxAsync & UxAsync::operator =(UxAsync const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxAsync & UxAsync::operator =(UxAsync && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxAsync::copy(UxAsync const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxAsync::swap(UxAsync & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxAsync::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

