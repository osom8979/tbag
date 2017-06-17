/**
 * @file   UdpReceiver.cpp
 * @brief  UdpReceiver class implementation.
 * @author zer0
 * @date   2017-06-17
 */

#include <libtbag/network/udp/UdpReceiver.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpReceiver::UdpReceiver(Loop & loop, ServerInterface * parent) : _parent(parent)
{
    _client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_client));
}

UdpReceiver::~UdpReceiver()
{
    // EMPTY.
}

UdpReceiver::Id UdpReceiver::id() const
{
}

std::string UdpReceiver::dest() const
{
}

int UdpReceiver::port() const
{
}

void * UdpReceiver::udata()
{
}

Err UdpReceiver::init(char const * destination, int port, uint64_t millisec)
{
}

Err UdpReceiver::start()
{
}

Err UdpReceiver::stop()
{
}

void UdpReceiver::close()
{
}

void UdpReceiver::cancel()
{
}

Err UdpReceiver::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
}

Err UdpReceiver::write(char const * buffer, std::size_t size, uint64_t millisec)
{
}

// --------------
// Event backend.
// --------------

void UdpReceiver::backConnect(Err code)
{
}

void UdpReceiver::backShutdown(Err code)
{
}

void UdpReceiver::backWrite(Err code)
{
}

void UdpReceiver::backRead(Err code, ReadPacket const & packet)
{
}

void UdpReceiver::backClose()
{
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

