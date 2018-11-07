/**
 * @file   UxLoop.cpp
 * @brief  UxLoop class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxLoop.hpp>
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
 * UxLoop::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxLoop::Impl : private Noncopyable
{
private:
    UxLoop * _parent;

public:
    Impl(UxLoop * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxLoop implementation.
// ----------------------------

UxLoop::UxLoop() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxLoop::UxLoop(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxLoop::UxLoop(UxLoop const & obj) TBAG_NOEXCEPT : UxLoop(nullptr)
{
    (*this) = obj;
}

UxLoop::UxLoop(UxLoop && obj) TBAG_NOEXCEPT : UxLoop(nullptr)
{
    (*this) = std::move(obj);
}

UxLoop::~UxLoop()
{
    // EMPTY.
}

UxLoop & UxLoop::operator =(UxLoop const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxLoop & UxLoop::operator =(UxLoop && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxLoop::copy(UxLoop const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxLoop::swap(UxLoop & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxLoop::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

