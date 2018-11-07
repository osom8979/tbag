/**
 * @file   UxUdp.cpp
 * @brief  UxUdp class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxUdp.hpp>
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
 * UxUdp::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxUdp::Impl : private Noncopyable
{
private:
    UxUdp * _parent;

public:
    Impl(UxUdp * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxUdp implementation.
// ----------------------------

UxUdp::UxUdp() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxUdp::UxUdp(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxUdp::UxUdp(UxUdp const & obj) TBAG_NOEXCEPT : UxUdp(nullptr)
{
    (*this) = obj;
}

UxUdp::UxUdp(UxUdp && obj) TBAG_NOEXCEPT : UxUdp(nullptr)
{
    (*this) = std::move(obj);
}

UxUdp::~UxUdp()
{
    // EMPTY.
}

UxUdp & UxUdp::operator =(UxUdp const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxUdp & UxUdp::operator =(UxUdp && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxUdp::copy(UxUdp const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxUdp::swap(UxUdp & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxUdp::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

