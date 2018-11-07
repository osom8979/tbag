/**
 * @file   UxSignal.cpp
 * @brief  UxSignal class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxSignal.hpp>
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
 * UxSignal::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxSignal::Impl : private Noncopyable
{
private:
    UxSignal * _parent;

public:
    Impl(UxSignal * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxSignal implementation.
// ----------------------------

UxSignal::UxSignal() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxSignal::UxSignal(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxSignal::UxSignal(UxSignal const & obj) TBAG_NOEXCEPT : UxSignal(nullptr)
{
    (*this) = obj;
}

UxSignal::UxSignal(UxSignal && obj) TBAG_NOEXCEPT : UxSignal(nullptr)
{
    (*this) = std::move(obj);
}

UxSignal::~UxSignal()
{
    // EMPTY.
}

UxSignal & UxSignal::operator =(UxSignal const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxSignal & UxSignal::operator =(UxSignal && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxSignal::copy(UxSignal const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxSignal::swap(UxSignal & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxSignal::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

