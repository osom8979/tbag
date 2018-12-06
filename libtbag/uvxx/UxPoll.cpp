/**
 * @file   UxPoll.cpp
 * @brief  UxPoll class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxPoll.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxPoll::UxPoll()
{
    // EMPTY.
}

UxPoll::UxPoll(UxLoop & loop, init_file fd)
{
    auto const CODE = initFile(loop, fd.fd);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxPoll::UxPoll(UxLoop & loop, init_sock sock)
{
    auto const CODE = initSock(loop, sock.sock);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxPoll::UxPoll(UxPoll const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxPoll::UxPoll(UxPoll && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxPoll::~UxPoll()
{
    // EMPTY.
}

UxPoll & UxPoll::operator =(UxPoll const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPoll & UxPoll::operator =(UxPoll && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPoll::copy(UxPoll const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxPoll::swap(UxPoll & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxPoll::initFile(UxLoop & loop, int fd)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncPoll>(*loop, init_file(fd));
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxPoll::initSock(UxLoop & loop, usock sock)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncPoll>(*loop, init_sock(sock));
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxPoll::start(EventType events)
{
    if (auto shared = lock()) {
        return shared->start(events);
    }
    return Err::E_EXPIRED;
}

Err UxPoll::stop()
{
    if (auto shared = lock()) {
        return shared->stop();
    }
    return Err::E_EXPIRED;
}

bool UxPoll::isEventReadable(EventType event) TBAG_NOEXCEPT
{
    return FuncPoll::isEventReadable(event);
}

bool UxPoll::isEventWritable(EventType event) TBAG_NOEXCEPT
{
    return FuncPoll::isEventWritable(event);
}

bool UxPoll::isEventDisconnect(EventType event) TBAG_NOEXCEPT
{
    return FuncPoll::isEventDisconnect(event);
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

