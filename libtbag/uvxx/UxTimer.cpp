/**
 * @file   UxTimer.cpp
 * @brief  UxTimer class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxTimer.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxTimer::UxTimer()
{
    // EMPTY.
}

UxTimer::UxTimer(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxTimer::UxTimer(UxTimer const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxTimer::UxTimer(UxTimer && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxTimer::~UxTimer()
{
    // EMPTY.
}

UxTimer & UxTimer::operator =(UxTimer const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTimer & UxTimer::operator =(UxTimer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTimer::copy(UxTimer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxTimer::swap(UxTimer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxTimer::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncTimer>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return E_SUCCESS;
}

Err UxTimer::start(uint64_t timeout, uint64_t repeat)
{
    if (auto shared = lock()) {
        return shared->start(timeout, repeat);
    }
    return E_EXPIRED;
}

Err UxTimer::stop()
{
    if (auto shared = lock()) {
        return shared->stop();
    }
    return E_EXPIRED;
}

Err UxTimer::again()
{
    if (auto shared = lock()) {
        return shared->again();
    }
    return E_EXPIRED;
}

void UxTimer::setRepeat(uint64_t repeat)
{
    if (auto shared = lock()) {
        shared->setRepeat(repeat);
    }
}

uint64_t UxTimer::getRepeat()
{
    if (auto shared = lock()) {
        return shared->getRepeat();
    }
    return 0;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

