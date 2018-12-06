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
    if (auto shared = _handle.lock()) {
        return shared->getName();
    }
    return "EXPIRED_HANDLE";
}

UxHandle::Loop * UxHandle::getLoop()
{
    if (auto shared = _handle.lock()) {
        return shared->getLoop();
    }
    return nullptr;
}

UxHandle::Loop const * UxHandle::getLoop() const
{
    if (auto shared = _handle.lock()) {
        return shared->getLoop();
    }
    return nullptr;
}

void UxHandle::close()
{
    if (auto shared = _handle.lock()) {
        shared->close();
    }
}

bool UxHandle::isInit() const
{
    if (auto shared = _handle.lock()) {
        return shared->isInit();
    }
    return false;
}

bool UxHandle::isActive() const
{
    if (auto shared = _handle.lock()) {
        return shared->isActive();
    }
    return false;
}

bool UxHandle::isClosing() const
{
    if (auto shared = _handle.lock()) {
        return shared->isClosing();
    }
    return true;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

