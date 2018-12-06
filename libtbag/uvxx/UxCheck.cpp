/**
 * @file   UxCheck.cpp
 * @brief  UxCheck class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxCheck.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxCheck::UxCheck()
{
    // EMPTY.
}

UxCheck::UxCheck(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxCheck::UxCheck(UxCheck const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxCheck::UxCheck(UxCheck && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxCheck::~UxCheck()
{
    // EMPTY.
}

UxCheck & UxCheck::operator =(UxCheck const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxCheck & UxCheck::operator =(UxCheck && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxCheck::copy(UxCheck const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxCheck::swap(UxCheck & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxCheck::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncCheck>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxCheck::start()
{
    if (auto shared = lock()) {
        return shared->start();
    }
    return Err::E_EXPIRED;
}

Err UxCheck::stop()
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

