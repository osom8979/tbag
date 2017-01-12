/**
 * @file   CommonUdp.cpp
 * @brief  CommonUdp class implementation.
 * @author zer0
 * @date   2017-01-12
 */

#include <libtbag/network/CommonUdp.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

CommonUdp::CommonUdp()
{
    // EMPTY.
}

CommonUdp::CommonUdp(SharedUdp udp) : _udp(udp)
{
    // EMPTY.
}

CommonUdp::CommonUdp(CallableUdp * udp) : _udp(udp)
{
    // EMPTY.
}

CommonUdp::CommonUdp(uv::Loop & loop) : _udp(new CallableUdp(loop, this))
{
    // EMPTY.
}

CommonUdp::~CommonUdp()
{
    // EMPTY.
}

CommonUdp::UdpSendRequest * CommonUdp::obtainUdpSendRequest()
{
    return static_cast<UdpSendRequest*>(_senders.create(_udp.get()).lock().get());
}

void CommonUdp::releaseUdpSendRequest(UdpSendRequest * request)
{
    _senders.release(static_cast<uv::Request*>(request));
}

CommonUdp::UdpSendRequest * CommonUdp::asyncWrite(binf * infos, std::size_t infos_size, sockaddr const * addr)
{
    if (static_cast<bool>(_udp)) {
        UdpSendRequest * request = obtainUdpSendRequest();
        if (request != nullptr) {
            if (_udp->send(*request, infos, infos_size, addr)) {
                return request;
            }
        }
    }
    return nullptr;
}

CommonUdp::UdpSendRequest * CommonUdp::asyncWrite(char const * buffer, std::size_t size, sockaddr const * addr)
{
    if (static_cast<bool>(_udp)) {
        UdpSendRequest * request = obtainUdpSendRequest();
        if (request != nullptr) {
            if (_udp->send(*request, buffer, size, addr)) {
                return request;
            }
        }
    }
    return nullptr;
}

std::size_t CommonUdp::tryWrite(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result)
{
    if (static_cast<bool>(_udp)) {
        return _udp->trySend(infos, infos_size, addr, result);
    }

    if (result != nullptr) {
        *result = Err::NULL_POINTER;
    }
    return 0U;
}

std::size_t CommonUdp::tryWrite(char const * buffer, std::size_t size, sockaddr const * addr, Err * result)
{
    if (static_cast<bool>(_udp)) {
        return _udp->trySend(buffer, size, addr, result);
    }

    if (result != nullptr) {
        *result = Err::NULL_POINTER;
    }
    return 0U;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

