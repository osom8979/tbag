/**
 * @file   UxFsPool.cpp
 * @brief  UxFsPool class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxFsPool.hpp>
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
 * UxFsPool::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxFsPool::Impl : private Noncopyable
{
private:
    UxFsPool * _parent;

public:
    Impl(UxFsPool * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxFsPool implementation.
// ----------------------------

UxFsPool::UxFsPool() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxFsPool::UxFsPool(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxFsPool::UxFsPool(UxFsPool const & obj) TBAG_NOEXCEPT : UxFsPool(nullptr)
{
    (*this) = obj;
}

UxFsPool::UxFsPool(UxFsPool && obj) TBAG_NOEXCEPT : UxFsPool(nullptr)
{
    (*this) = std::move(obj);
}

UxFsPool::~UxFsPool()
{
    // EMPTY.
}

UxFsPool & UxFsPool::operator =(UxFsPool const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxFsPool & UxFsPool::operator =(UxFsPool && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxFsPool::copy(UxFsPool const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxFsPool::swap(UxFsPool & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxFsPool::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

