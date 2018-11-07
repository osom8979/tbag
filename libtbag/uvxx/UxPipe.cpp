/**
 * @file   UxPipe.cpp
 * @brief  UxPipe class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxPipe.hpp>
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
 * UxPipe::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxPipe::Impl : private Noncopyable
{
private:
    UxPipe * _parent;

public:
    Impl(UxPipe * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxPipe implementation.
// ----------------------------

UxPipe::UxPipe() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxPipe::UxPipe(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxPipe::UxPipe(UxPipe const & obj) TBAG_NOEXCEPT : UxPipe(nullptr)
{
    (*this) = obj;
}

UxPipe::UxPipe(UxPipe && obj) TBAG_NOEXCEPT : UxPipe(nullptr)
{
    (*this) = std::move(obj);
}

UxPipe::~UxPipe()
{
    // EMPTY.
}

UxPipe & UxPipe::operator =(UxPipe const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPipe & UxPipe::operator =(UxPipe && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPipe::copy(UxPipe const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxPipe::swap(UxPipe & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxPipe::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

