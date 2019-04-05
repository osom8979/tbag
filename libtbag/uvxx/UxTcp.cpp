/**
 * @file   UxTcp.cpp
 * @brief  UxTcp class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxTcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxTcp::UxTcp()
{
    // EMPTY.
}

UxTcp::UxTcp(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxTcp::UxTcp(UxTcp const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxTcp::UxTcp(UxTcp && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxTcp::~UxTcp()
{
    // EMPTY.
}

UxTcp & UxTcp::operator =(UxTcp const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTcp & UxTcp::operator =(UxTcp && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTcp::copy(UxTcp const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::copy(obj);
    }
}

void UxTcp::swap(UxTcp & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::swap(obj);
    }
}

Err UxTcp::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncTcp>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return E_SUCCESS;
}

Err UxTcp::setNodelay(bool enable)
{
    if (auto shared = lock()) {
        return shared->setNodelay(enable);
    }
    return E_EXPIRED;
}

Err UxTcp::keepAlive(bool enable, unsigned int delay)
{
    if (auto shared = lock()) {
        return shared->keepAlive(enable, delay);
    }
    return E_EXPIRED;
}

Err UxTcp::acceptsSimultaneous(int enable)
{
    if (auto shared = lock()) {
        return shared->acceptsSimultaneous(enable);
    }
    return E_EXPIRED;
}

Err UxTcp::bind(sockaddr const * address, unsigned int flags)
{
    if (auto shared = lock()) {
        return shared->bind(address, flags);
    }
    return E_EXPIRED;
}

Err UxTcp::bind(sockaddr const * address, BindFlag flags)
{
    if (auto shared = lock()) {
        return shared->bind(address, flags);
    }
    return E_EXPIRED;
}

Err UxTcp::getSockName(struct sockaddr * name, int * namelen)
{
    if (auto shared = lock()) {
        return shared->getSockName(name, namelen);
    }
    return E_EXPIRED;
}

Err UxTcp::getPeerName(struct sockaddr * name, int * namelen)
{
    if (auto shared = lock()) {
        return shared->getPeerName(name, namelen);
    }
    return E_EXPIRED;
}

std::string UxTcp::getSockIp()
{
    if (auto shared = lock()) {
        return shared->getSockIp();
    }
    return std::string();
}

std::string UxTcp::getPeerIp()
{
    if (auto shared = lock()) {
        return shared->getPeerIp();
    }
    return std::string();
}

int UxTcp::getSockPort()
{
    if (auto shared = lock()) {
        return shared->getSockPort();
    }
    return 0;
}

int UxTcp::getPeerPort()
{
    if (auto shared = lock()) {
        return shared->getPeerPort();
    }
    return 0;
}

Err UxTcp::connect(ConnectRequest & request, sockaddr const * address)
{
    if (auto shared = lock()) {
        return shared->connect(request, address);
    }
    return E_EXPIRED;
}

// ----------------
// Utility methods.
// ----------------

Err initCommonClientSock(UxTcp & tcp, UxTcp::ConnectRequest & request, struct sockaddr const * addr)
{
    if (auto shared = tcp.lock()) {
        return libtbag::uvpp::initCommonClientSock(*shared, request, addr);
    }
    return E_EXPIRED;
}

Err initCommonClient(UxTcp & tcp, UxTcp::ConnectRequest & request, std::string const & host, int port)
{
    if (auto shared = tcp.lock()) {
        return libtbag::uvpp::initCommonClient(*shared, request, host, port);
    }
    return E_EXPIRED;
}

Err initCommonServerSock(UxTcp & tcp, struct sockaddr const * addr)
{
    if (auto shared = tcp.lock()) {
        return libtbag::uvpp::initCommonServerSock(*shared, addr);
    }
    return E_EXPIRED;
}

Err initCommonServer(UxTcp & tcp, std::string const & ip, int port)
{
    if (auto shared = tcp.lock()) {
        return libtbag::uvpp::initCommonServer(*shared, ip, port);
    }
    return E_EXPIRED;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

