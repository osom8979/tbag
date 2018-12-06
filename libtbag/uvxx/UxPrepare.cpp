/**
 * @file   UxPrepare.cpp
 * @brief  UxPrepare class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxPrepare.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxPrepare::UxPrepare() : UxHandle()
{
    // EMPTY.
}

UxPrepare::UxPrepare(UxLoop & loop) : UxHandle()
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxPrepare::UxPrepare(UxPrepare const & obj) TBAG_NOEXCEPT : UxPrepare()
{
    (*this) = obj;
}

UxPrepare::UxPrepare(UxPrepare && obj) TBAG_NOEXCEPT : UxPrepare()
{
    (*this) = std::move(obj);
}

UxPrepare::~UxPrepare()
{
    // EMPTY.
}

UxPrepare & UxPrepare::operator =(UxPrepare const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPrepare & UxPrepare::operator =(UxPrepare && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPrepare::copy(UxPrepare const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxPrepare::swap(UxPrepare & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxPrepare::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncPrepare>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxPrepare::start()
{
    if (auto shared = lock()) {
        return shared->start();
    }
    return Err::E_EXPIRED;
}

Err UxPrepare::stop()
{
    if (auto shared = lock()) {
        return shared->stop();
    }
    return Err::E_EXPIRED;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

