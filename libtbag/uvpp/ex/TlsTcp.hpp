/**
 * @file   TlsTcp.hpp
 * @brief  TlsTcp class prototype.
 * @author zer0
 * @date   2019-01-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TLSTCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TLSTCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/crypto/Tls.hpp>
#include <libtbag/uvpp/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

/**
 * TlsTcp class prototype.
 *
 * @author zer0
 * @date   2019-01-26
 */
class TBAG_API TlsTcp : public Tcp
{
private:

public:
    TlsTcp();
    TlsTcp(Loop & loop);
    virtual ~TlsTcp();

public:
    /** Establish an IPv4 or IPv6 TCP/TLS connection. */
    Err connectTls(ConnectRequest & request, sockaddr const * address);

    /** Start TLS listening for incoming connections. */
    Err listenTls(int backlog = BACKLOG_LIMIT);

    /** Read TLS data from an incoming stream. */
    Err startReadTls();

    /** Write TLS data to stream. Buffers are written in order. */
    Err writeTls(WriteRequest & request, binf const * infos, std::size_t infos_size);
    Err writeTls(WriteRequest & request, char const * buffer, std::size_t size);

    std::size_t tryWriteTls(binf * infos, std::size_t infos_size, Err * result = nullptr);
    std::size_t tryWriteTls(char const * buffer, std::size_t size, Err * result = nullptr);
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_TLSTCP_HPP__

