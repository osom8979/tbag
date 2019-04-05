/**
 * @file   UxFsEvent.cpp
 * @brief  UxFsEvent class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxFsEvent.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxFsEvent::UxFsEvent()
{
    // EMPTY.
}

UxFsEvent::UxFsEvent(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxFsEvent::UxFsEvent(UxFsEvent const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxFsEvent::UxFsEvent(UxFsEvent && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxFsEvent::~UxFsEvent()
{
    // EMPTY.
}

UxFsEvent & UxFsEvent::operator =(UxFsEvent const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxFsEvent & UxFsEvent::operator =(UxFsEvent && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxFsEvent::copy(UxFsEvent const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxFsEvent::swap(UxFsEvent & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxFsEvent::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncFsEvent>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return E_SUCCESS;
}

Err UxFsEvent::start(char const * path, EventFlag flags)
{
    if (auto shared = lock()) {
        return shared->start(path, flags);
    }
    return E_EXPIRED;
}

Err UxFsEvent::stop()
{
    if (auto shared = lock()) {
        return shared->stop();
    }
    return E_EXPIRED;
}

std::string UxFsEvent::getPath()
{
    if (auto shared = lock()) {
        return shared->getPath();
    }
    return std::string();
}

UxFsEvent::Event UxFsEvent::getEvent(int native_events) TBAG_NOEXCEPT
{
    return FuncFsEvent::getEvent(native_events);
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

