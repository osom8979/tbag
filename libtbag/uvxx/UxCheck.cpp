/**
 * @file   UxCheck.cpp
 * @brief  UxCheck class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxCheck.hpp>
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
 * UxCheck::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxCheck::Impl : private Noncopyable
{
private:
    UxCheck * _parent;

public:
    Impl(UxCheck * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxCheck implementation.
// ----------------------------

UxCheck::UxCheck() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxCheck::UxCheck(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxCheck::UxCheck(UxCheck const & obj) TBAG_NOEXCEPT : UxCheck(nullptr)
{
    (*this) = obj;
}

UxCheck::UxCheck(UxCheck && obj) TBAG_NOEXCEPT : UxCheck(nullptr)
{
    (*this) = std::move(obj);
}

UxCheck::~UxCheck()
{
    // EMPTY.
}

UxCheck & UxCheck::operator =(UxCheck const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxCheck & UxCheck::operator =(UxCheck && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxCheck::copy(UxCheck const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxCheck::swap(UxCheck & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxCheck::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

