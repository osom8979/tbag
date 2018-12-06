/**
 * @file   UxHandle.cpp
 * @brief  UxHandle class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxHandle::UxHandle() : _handle()
{
    // EMPTY.
}

UxHandle::UxHandle(UxHandle const & obj) TBAG_NOEXCEPT : UxHandle()
{
    (*this) = obj;
}

UxHandle::UxHandle(UxHandle && obj) TBAG_NOEXCEPT : UxHandle()
{
    (*this) = std::move(obj);
}

UxHandle::~UxHandle()
{
    // EMPTY.
}

UxHandle & UxHandle::operator =(UxHandle const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxHandle & UxHandle::operator =(UxHandle && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxHandle::copy(UxHandle const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _handle = obj._handle;
    }
}

void UxHandle::swap(UxHandle & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _handle.swap(obj._handle);
    }
}

void UxHandle::release()
{
    if (auto shared = _handle.lock()) {
        if (shared->isInit() && !shared->isClosing()) {
            shared->tryClose();
        }
        _handle.reset();
    }
}

char const * UxHandle::getName() const
{
    return _handle.lock()->getName();
}

UxHandle::Loop * UxHandle::getLoop()
{
    return _handle.lock()->getLoop();
}

UxHandle::Loop const * UxHandle::getLoop() const
{
    return _handle.lock()->getLoop();
}

void UxHandle::close()
{
    _handle.lock()->close();
}

bool UxHandle::isInit() const
{
    return _handle.lock()->isInit();
}

bool UxHandle::isActive() const
{
    return _handle.lock()->isActive();
}

bool UxHandle::isClosing() const
{
    return _handle.lock()->isClosing();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

