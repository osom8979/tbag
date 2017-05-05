/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2017-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/network/details/NetCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class TcpRealClient;
class TcpClient;

/**
 * TcpRealClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealClient : public details::NetCommon, public uvpp::Tcp
{
public:
    friend class TcpClient;

private:
    TcpClient    & _parent;
    ConnectRequest _connect_req;
    WriteRequest   _write_req;
    Buffer         _buffer;

public:
    TcpRealClient(Loop & loop, TcpClient & parent);
    virtual ~TcpRealClient();

public:
    virtual void onConnect(ConnectRequest & request, uerr code) override;
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpClient : public Client
{
public:
    TcpClient(Loop & loop);
    virtual ~TcpClient();

public:
    virtual Type getType() const
    { return Type::TCP; }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__

