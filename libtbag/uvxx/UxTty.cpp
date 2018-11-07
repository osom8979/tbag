/**
 * @file   UxTty.cpp
 * @brief  UxTty class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxTty.hpp>
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
 * UxTty::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxTty::Impl : private Noncopyable
{
private:
    UxTty * _parent;

public:
    Impl(UxTty * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxTty implementation.
// ----------------------------

UxTty::UxTty() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxTty::UxTty(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxTty::UxTty(UxTty const & obj) TBAG_NOEXCEPT : UxTty(nullptr)
{
    (*this) = obj;
}

UxTty::UxTty(UxTty && obj) TBAG_NOEXCEPT : UxTty(nullptr)
{
    (*this) = std::move(obj);
}

UxTty::~UxTty()
{
    // EMPTY.
}

UxTty & UxTty::operator =(UxTty const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTty & UxTty::operator =(UxTty && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTty::copy(UxTty const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxTty::swap(UxTty & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxTty::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

