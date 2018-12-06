/**
 * @file   UxSignal.cpp
 * @brief  UxSignal class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxSignal.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxSignal::UxSignal()
{
    // EMPTY.
}

UxSignal::UxSignal(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxSignal::UxSignal(UxSignal const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxSignal::UxSignal(UxSignal && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxSignal::~UxSignal()
{
    // EMPTY.
}

UxSignal & UxSignal::operator =(UxSignal const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxSignal & UxSignal::operator =(UxSignal && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxSignal::copy(UxSignal const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxSignal::swap(UxSignal & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxSignal::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncSignal>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

int UxSignal::getSignalNumber() const TBAG_NOEXCEPT
{
    if (auto shared = lock()) {
        return shared->getSignalNumber();
    }
    return 0;
}

Err UxSignal::start(int signum)
{
    if (auto shared = lock()) {
        return shared->start(signum);
    }
    return Err::E_EXPIRED;
}

Err UxSignal::startOneshot(int signum)
{
    if (auto shared = lock()) {
        return shared->startOneshot(signum);
    }
    return Err::E_EXPIRED;
}

Err UxSignal::stop()
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

