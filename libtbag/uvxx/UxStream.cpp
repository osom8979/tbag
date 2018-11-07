/**
 * @file   UxStream.cpp
 * @brief  UxStream class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxStream.hpp>
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
 * UxStream::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxStream::Impl : private Noncopyable
{
private:
    UxStream * _parent;

public:
    Impl(UxStream * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxStream implementation.
// ----------------------------

UxStream::UxStream() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxStream::UxStream(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxStream::UxStream(UxStream const & obj) TBAG_NOEXCEPT : UxStream(nullptr)
{
    (*this) = obj;
}

UxStream::UxStream(UxStream && obj) TBAG_NOEXCEPT : UxStream(nullptr)
{
    (*this) = std::move(obj);
}

UxStream::~UxStream()
{
    // EMPTY.
}

UxStream & UxStream::operator =(UxStream const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxStream & UxStream::operator =(UxStream && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxStream::copy(UxStream const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxStream::swap(UxStream & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxStream::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

