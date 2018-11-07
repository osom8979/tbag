/**
 * @file   UxRequest.cpp
 * @brief  UxRequest class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxRequest.hpp>
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
 * UxRequest::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxRequest::Impl : private Noncopyable
{
private:
    UxRequest * _parent;

public:
    Impl(UxRequest * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxRequest implementation.
// ----------------------------

UxRequest::UxRequest() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxRequest::UxRequest(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxRequest::UxRequest(UxRequest const & obj) TBAG_NOEXCEPT : UxRequest(nullptr)
{
    (*this) = obj;
}

UxRequest::UxRequest(UxRequest && obj) TBAG_NOEXCEPT : UxRequest(nullptr)
{
    (*this) = std::move(obj);
}

UxRequest::~UxRequest()
{
    // EMPTY.
}

UxRequest & UxRequest::operator =(UxRequest const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxRequest & UxRequest::operator =(UxRequest && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxRequest::copy(UxRequest const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxRequest::swap(UxRequest & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxRequest::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

