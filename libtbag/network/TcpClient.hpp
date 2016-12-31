/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2016-12-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/TcpLoop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class TBAG_API TcpClient : public TcpLoop
{
public:
    using Parent = TcpLoop;

private:
    ConnectRequest _connector;
    Buffer _read_buffer;

public:
    TcpClient();
    virtual ~TcpClient();

public:
    bool initIpv4(std::string const & ip, int port);

public:
    bool asyncWrite(binf * infos, std::size_t infos_size);
    bool asyncWrite(char const * buffer, std::size_t size);

public:
    virtual bool run(std::string const & ip, int port) override;

public:
    virtual binf onAlloc(std::size_t suggested_size) override;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

