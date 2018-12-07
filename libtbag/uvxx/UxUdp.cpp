/**
 * @file   UxUdp.cpp
 * @brief  UxUdp class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxUdp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxUdp::UxUdp()
{
    // EMPTY.
}

UxUdp::UxUdp(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxUdp::UxUdp(UxUdp const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxUdp::UxUdp(UxUdp && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxUdp::~UxUdp()
{
    // EMPTY.
}

UxUdp & UxUdp::operator =(UxUdp const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxUdp & UxUdp::operator =(UxUdp && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxUdp::copy(UxUdp const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxUdp::swap(UxUdp & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxUdp::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncUdp>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

std::size_t UxUdp::getSendQueueSize() const
{
    if (auto shared = lock()) {
        return shared->getSendQueueSize();
    }
    return 0;
}

std::size_t UxUdp::getSendQueueCount() const
{
    if (auto shared = lock()) {
        return shared->getSendQueueCount();
    }
    return 0;
}

Err UxUdp::open(usock sock)
{
    if (auto shared = lock()) {
        return shared->open(sock);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::bind(sockaddr const * addr, unsigned int flags)
{
    if (auto shared = lock()) {
        return shared->bind(addr, flags);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::getSockName(sockaddr * name, int * namelen)
{
    if (auto shared = lock()) {
        return shared->getSockName(name, namelen);
    }
    return Err::E_EXPIRED;
}

std::string UxUdp::getSockIp()
{
    if (auto shared = lock()) {
        return shared->getSockIp();
    }
    return std::string();
}

int UxUdp::getSockPort()
{
    if (auto shared = lock()) {
        return shared->getSockPort();
    }
    return 0;
}

Err UxUdp::setMembership(char const * multicast_addr, char const * interface_addr, Membership membership)
{
    if (auto shared = lock()) {
        return shared->setMembership(multicast_addr, interface_addr, membership);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::setMulticastLoop(bool on)
{
    if (auto shared = lock()) {
        return shared->setMulticastLoop(on);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::setMulticastTtl(int ttl)
{
    if (auto shared = lock()) {
        return shared->setMulticastTtl(ttl);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::setMulticastInterface(char const * interface_addr)
{
    if (auto shared = lock()) {
        return shared->setMulticastInterface(interface_addr);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::setBroadcast(bool on)
{
    if (auto shared = lock()) {
        return shared->setBroadcast(on);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::setTtl(int ttl)
{
    if (auto shared = lock()) {
        return shared->setTtl(ttl);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr const * addr)
{
    if (auto shared = lock()) {
        return shared->send(request, infos, infos_size, addr);
    }
    return Err::E_EXPIRED;
}

Err UxUdp::send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr)
{
    if (auto shared = lock()) {
        return shared->send(request, buffer, size, addr);
    }
    return Err::E_EXPIRED;
}

std::size_t UxUdp::trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result)
{
    if (auto shared = lock()) {
        return shared->trySend(infos, infos_size, addr, result);
    }
    if (result != nullptr) {
        *result = Err::E_EXPIRED;
    }
    return 0;
}

std::size_t UxUdp::trySend(char const * buffer, std::size_t size, sockaddr const * addr, Err * result)
{
    if (auto shared = lock()) {
        return shared->trySend(buffer, size, addr, result);
    }
    if (result != nullptr) {
        *result = Err::E_EXPIRED;
    }
    return 0;
}

Err UxUdp::startRecv()
{
    if (auto shared = lock()) {
        return shared->startRecv();
    }
    return Err::E_EXPIRED;
}

Err UxUdp::stopRecv()
{
    if (auto shared = lock()) {
        return shared->stopRecv();
    }
    return Err::E_EXPIRED;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

