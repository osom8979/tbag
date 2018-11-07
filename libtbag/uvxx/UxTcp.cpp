/**
 * @file   UxTcp.cpp
 * @brief  UxTcp class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxTcp.hpp>
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
 * UxTcp::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxTcp::Impl : private Noncopyable
{
private:
    UxTcp * _parent;

public:
    Impl(UxTcp * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxTcp implementation.
// ----------------------------

UxTcp::UxTcp() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxTcp::UxTcp(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxTcp::UxTcp(UxTcp const & obj) TBAG_NOEXCEPT : UxTcp(nullptr)
{
    (*this) = obj;
}

UxTcp::UxTcp(UxTcp && obj) TBAG_NOEXCEPT : UxTcp(nullptr)
{
    (*this) = std::move(obj);
}

UxTcp::~UxTcp()
{
    // EMPTY.
}

UxTcp & UxTcp::operator =(UxTcp const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTcp & UxTcp::operator =(UxTcp && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTcp::copy(UxTcp const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxTcp::swap(UxTcp & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxTcp::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

