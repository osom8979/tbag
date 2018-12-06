/**
 * @file   UxAsync.cpp
 * @brief  UxAsync class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxAsync.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxAsync::UxAsync() : UxHandle()
{
    // EMPTY.
}

UxAsync::UxAsync(UxLoop & loop) : UxHandle()
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxAsync::UxAsync(UxAsync const & obj) TBAG_NOEXCEPT : UxAsync()
{
    (*this) = obj;
}

UxAsync::UxAsync(UxAsync && obj) TBAG_NOEXCEPT : UxAsync()
{
    (*this) = std::move(obj);
}

UxAsync::~UxAsync()
{
    // EMPTY.
}

UxAsync & UxAsync::operator =(UxAsync const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxAsync & UxAsync::operator =(UxAsync && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxAsync::copy(UxAsync const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxAsync::swap(UxAsync & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxAsync::init(UxLoop & loop)
{
    try {
        UxHandle::release();
        _handle = loop->newHandle<FuncAsync>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxAsync::send()
{
    return lock()->send();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

