/**
 * @file   UdpNodeBackend.cpp
 * @brief  UdpNodeBackend class implementation.
 * @author zer0
 * @date   2017-06-15
 */

#include <libtbag/network/udp/UdpNodeBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpNodeBackend::UdpNodeBackend(Loop & loop, ClientInterface * parent) : Parent(loop), _parent(parent)
{
    // EMPTY.
}

UdpNodeBackend::~UdpNodeBackend()
{
    // EMPTY.
}

void UdpNodeBackend::onSend(UdpSendRequest & request, Err code)
{
    assert(_parent != nullptr);
    _parent->backWrite(code);
}

UdpNodeBackend::binf UdpNodeBackend::onAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_buffer, suggested_size);
}

void UdpNodeBackend::onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
{
    assert(_parent != nullptr);
    _parent->backRead(code, ReadPacket(buffer, size, addr, flags));
}

void UdpNodeBackend::onClose()
{
    assert(_parent != nullptr);
    _parent->backClose();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

