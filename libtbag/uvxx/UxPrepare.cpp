/**
 * @file   UxPrepare.cpp
 * @brief  UxPrepare class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxPrepare.hpp>
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
 * UxPrepare::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxPrepare::Impl : private Noncopyable
{
private:
    UxPrepare * _parent;

public:
    Impl(UxPrepare * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxPrepare implementation.
// ----------------------------

UxPrepare::UxPrepare() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxPrepare::UxPrepare(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxPrepare::UxPrepare(UxPrepare const & obj) TBAG_NOEXCEPT : UxPrepare(nullptr)
{
    (*this) = obj;
}

UxPrepare::UxPrepare(UxPrepare && obj) TBAG_NOEXCEPT : UxPrepare(nullptr)
{
    (*this) = std::move(obj);
}

UxPrepare::~UxPrepare()
{
    // EMPTY.
}

UxPrepare & UxPrepare::operator =(UxPrepare const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPrepare & UxPrepare::operator =(UxPrepare && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPrepare::copy(UxPrepare const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxPrepare::swap(UxPrepare & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxPrepare::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

