/**
 * @file   UxDns.cpp
 * @brief  UxDns class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxDns.hpp>
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
 * UxDns::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxDns::Impl : private Noncopyable
{
private:
    UxDns * _parent;

public:
    Impl(UxDns * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxDns implementation.
// ----------------------------

UxDns::UxDns() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxDns::UxDns(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxDns::UxDns(UxDns const & obj) TBAG_NOEXCEPT : UxDns(nullptr)
{
    (*this) = obj;
}

UxDns::UxDns(UxDns && obj) TBAG_NOEXCEPT : UxDns(nullptr)
{
    (*this) = std::move(obj);
}

UxDns::~UxDns()
{
    // EMPTY.
}

UxDns & UxDns::operator =(UxDns const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxDns & UxDns::operator =(UxDns && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxDns::copy(UxDns const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxDns::swap(UxDns & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxDns::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

