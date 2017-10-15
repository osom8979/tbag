/**
 * @file   UdpEcho.cpp
 * @brief  UdpEcho class implementation.
 * @author zer0
 * @date   2017-08-14
 */

#include <libtbag/network/udp/UdpEcho.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpEcho::UdpEcho(Loop & loop)
        : UdpNode(loop, UdpNode::UDP_NODE_FLAG_USE_BIND),
          _type(EchoType::ET_UNKNOWN), _timer(loop.newHandle<FuncTimer>(loop))
{
    // EMPTY.
}

UdpEcho::UdpEcho(Loop & loop, SharedSafetyAsync async, send_flags const & UNUSED_PARAM(send))
        : UdpNode(loop, async, UdpNode::send_flags()),
          _type(EchoType::ET_UNKNOWN), _timer(loop.newHandle<FuncTimer>(loop))
{
    // EMPTY.
}

UdpEcho::UdpEcho(Loop & loop, SharedSafetyAsync async, recv_flags const & UNUSED_PARAM(recv))
        : UdpNode(loop, async, UdpNode::recv_flags()),
          _type(EchoType::ET_UNKNOWN), _timer(loop.newHandle<FuncTimer>(loop))
{
    // EMPTY.
}

UdpEcho::UdpEcho(Loop & loop, SharedSafetyAsync async, unsigned int flags)
        : UdpNode(loop, async, flags),
          _type(EchoType::ET_UNKNOWN), _timer(loop.newHandle<FuncTimer>(loop))
{
    // EMPTY.
}

UdpEcho::~UdpEcho()
{
    // EMPTY.
}

Err UdpEcho::initDefault(std::string const & bind_ip, int port, uint64_t timeout)
{
    Err const INIT_CODE = UdpNode::init(bind_ip.c_str(), port);
    if (isFailure(INIT_CODE)) {
        return INIT_CODE;
    }

    Err const BROADCAST_CODE = UdpNode::setBroadcast();
    if (isFailure(BROADCAST_CODE)) {
        return BROADCAST_CODE;
    }

    Err const START_CODE = UdpNode::start();
    if (isFailure(START_CODE)) {
        return START_CODE;
    }

    if (timeout > 0) {
        _timer->setOnTimer([&](){
            close();
        });
        Err const START_TIMER_CODE = _timer->start(timeout);
        if (isFailure(START_TIMER_CODE)) {
            return START_TIMER_CODE;
        }
    }

    return Err::E_SUCCESS;
}

Err UdpEcho::initServer(std::string const & bind_ip, int port, uint64_t timeout)
{
    _type = EchoType::ET_SERVER;
    return initDefault(bind_ip, port, timeout);
}

Err UdpEcho::initClient(std::string const & bind_ip, int port, int broadcast_port, uint64_t timeout)
{
    _type = EchoType::ET_CLIENT;
    Err const SET_DEST_CODE = UdpNode::setDestination(details::BROADCAST_SUBNET_IPV4, broadcast_port);
    if (isFailure(SET_DEST_CODE)) {
        return SET_DEST_CODE;
    }
    return initDefault(bind_ip, port, timeout);
}

Err UdpEcho::echo(std::string const & message)
{
    return UdpNode::write(message.c_str(), message.size());
}

void UdpEcho::onWrite(Err code)
{
    tDLogD("UdpEcho::onWrite({})", getErrName(code));
}

void UdpEcho::onRead(Err code, ReadPacket const & packet)
{
    assert(packet.type == details::PacketType::PT_DATAGRAM);
    if (code == Err::E_SUCCESS && packet.size > 0 && packet.addr != nullptr) {

        SocketAddress addr(packet.addr);
        std::string message(packet.buffer, packet.buffer + packet.size);

        if (_type == EchoType::ET_SERVER) {
            auto client = getClient().lock();
            assert(static_cast<bool>(client));

            Loop * loop = client->getLoop();
            assert(loop != nullptr);

            auto udp = loop->newHandle<uvpp::Udp>(*loop);
            assert(static_cast<bool>(udp));

            onEcho(message, addr);

            Err send_result = Err::E_UNKNOWN;
            auto send_size = udp->trySend(message.data(), message.size(), packet.addr, &send_result);

            if (send_size != packet.size || isFailure(send_result)) {
                tDLogE("UdpEcho::onRead() try_send error (size: {}, code: {})", send_size, getErrName(send_result));
            }
            udp->close();
        } else {
            onEcho(message, addr);
        }
    }
}

void UdpEcho::onClose()
{
    _timer->close();
    onEnd();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

