/**
 * @file   UxIdle.cpp
 * @brief  UxIdle class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxIdle.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxIdle::UxIdle()
{
    // EMPTY.
}

UxIdle::UxIdle(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxIdle::UxIdle(UxIdle const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxIdle::UxIdle(UxIdle && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxIdle::~UxIdle()
{
    // EMPTY.
}

UxIdle & UxIdle::operator =(UxIdle const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxIdle & UxIdle::operator =(UxIdle && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxIdle::copy(UxIdle const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxIdle::swap(UxIdle & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxIdle::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncIdle>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxIdle::start()
{
    if (auto shared = lock()) {
        return shared->start();
    }
    return Err::E_EXPIRED;
}

Err UxIdle::stop()
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

