/**
 * @file   UdpSender.cpp
 * @brief  UdpSender class implementation.
 * @author zer0
 * @date   2017-06-17
 */

#include <libtbag/network/udp/UdpSender.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpSender::UdpSender(Loop & loop, SharedSafetyAsync async, ServerInterface * parent)
        : _parent(parent)
{
    _client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async.reset();
    }

    _sender.status = SendStatus::SS_NOT_READY;
}

UdpSender::~UdpSender()
{
    // EMPTY.
}

char const * UdpSender::getSendStatusName(SendStatus status) TBAG_NOEXCEPT
{
    switch (status) {
    case SendStatus::SS_NOT_READY:     return "NOT_READY";
    case SendStatus::SS_READY:         return "READY";
    case SendStatus::SS_ASYNC:         return "ASYNC";
    case SendStatus::SS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case SendStatus::SS_SEND:          return "SEND";
    case SendStatus::SS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

void UdpSender::onAsyncSend()
{
}

UdpSender::Id UdpSender::id() const
{
}

std::string UdpSender::dest() const
{
}

int UdpSender::port() const
{
}

void * UdpSender::udata()
{
}

Err UdpSender::init(char const * destination, int port, uint64_t millisec)
{
}

Err UdpSender::start()
{
}

Err UdpSender::stop()
{
}

void UdpSender::close()
{
}

void UdpSender::cancel()
{
}

Err UdpSender::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
}

Err UdpSender::write(char const * buffer, std::size_t size, uint64_t millisec)
{
}

// --------------
// Event backend.
// --------------

void UdpSender::backConnect(Err code)
{
}

void UdpSender::backShutdown(Err code)
{
}

void UdpSender::backWrite(Err code)
{
}

void UdpSender::backRead(Err code, ReadPacket const & packet)
{
}

void UdpSender::backClose()
{
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

