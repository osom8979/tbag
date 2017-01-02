/**
 * @file   TcpLoop.hpp
 * @brief  TcpLoop class prototype.
 * @author zer0
 * @date   2016-12-29
 * @date   2016-12-31 (Craete BaseTcpLoop)
 * @date   2016-01-02 (Remove BaseTcpLoop)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/CommonTcp.hpp>
#include <libtbag/uv/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpLoop class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class TBAG_API TcpLoop : public CommonTcp
{
public:
    using Parent = CommonTcp;

public:
    uv::Loop _loop;

public:
    TcpLoop();
    virtual ~TcpLoop();

public:
    // @formatter:off
    inline uv::Loop       & atLoop()       TBAG_NOEXCEPT { return _loop; }
    inline uv::Loop const & atLoop() const TBAG_NOEXCEPT { return _loop; }
    // @formatter:on

public:
    virtual bool run(std::string const & ip, int port) = 0;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

