/**
 * @file   UxProcess.cpp
 * @brief  UxProcess class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxProcess.hpp>
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
 * UxProcess::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-07
 */
struct UxProcess::Impl : private Noncopyable
{
private:
    UxProcess * _parent;

public:
    Impl(UxProcess * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------------
// UxProcess implementation.
// ----------------------------

UxProcess::UxProcess() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

UxProcess::UxProcess(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

UxProcess::UxProcess(UxProcess const & obj) TBAG_NOEXCEPT : UxProcess(nullptr)
{
    (*this) = obj;
}

UxProcess::UxProcess(UxProcess && obj) TBAG_NOEXCEPT : UxProcess(nullptr)
{
    (*this) = std::move(obj);
}

UxProcess::~UxProcess()
{
    // EMPTY.
}

UxProcess & UxProcess::operator =(UxProcess const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxProcess & UxProcess::operator =(UxProcess && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxProcess::copy(UxProcess const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void UxProcess::swap(UxProcess & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void UxProcess::reset()
{
    _impl.reset();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

