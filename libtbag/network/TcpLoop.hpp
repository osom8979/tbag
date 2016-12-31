/**
 * @file   TcpLoop.hpp
 * @brief  TcpLoop class prototype.
 * @author zer0
 * @date   2016-12-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/ex/CallableTcp.hpp>
#include <libtbag/uv/Request.hpp>
#include <libtbag/uv/Async.hpp>
#include <libtbag/uv/ex/RequestQueue.hpp>

#include <cassert>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * BaseTcpLoop class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class TBAG_API BaseTcpLoop : public uv::ex::CallableTcp::Callback
{
public:
    using binf = uv::binf;
    using Loop = uv::Loop;

    using Tcp         = uv::Tcp;
    using CallableTcp = uv::ex::CallableTcp;
    using Callback    = CallableTcp::Callback;
    using SharedTcp   = std::shared_ptr<CallableTcp>;

    using ShutdownRequest = uv::ShutdownRequest;
    using ConnectRequest  = uv::ConnectRequest;
    using WriteRequest    = uv::WriteRequest;

    using Buffer = std::vector<char>;

public:
    Loop _loop;
    SharedTcp _tcp;

public:
    BaseTcpLoop() : _loop(), _tcp()
    {
        _tcp = _loop.createAndInsertChildHandle<CallableTcp>(_loop, this);
        assert(static_cast<bool>(_tcp));
    }

    virtual ~BaseTcpLoop()
    { /* EMPTY. */ }


public:
    // @formatter:off
    inline Loop        & atLoop() TBAG_NOEXCEPT { return _loop; }
    inline CallableTcp & atTcp () TBAG_NOEXCEPT { return *_tcp; }
    // @formatter:on

public:
    // @formatter:off
    inline void close    () { return _tcp->close();     }
    inline bool startRead() { return _tcp->startRead(); }
    inline bool stopRead () { return _tcp->stopRead (); }
    // @formatter:on

public:
    // @formatter:off
    inline bool asyncWrite(CallableTcp & tcp, WriteRequest & req, binf * infos, std::size_t infos_size)
    { return tcp.write(req, infos, infos_size); }
    inline bool asyncWrite(CallableTcp & tcp, WriteRequest & req, char const * buffer, std::size_t size)
    { return tcp.write(req, buffer, size); }

    inline std::size_t tryWrite(CallableTcp & tcp, binf * infos, std::size_t infos_size, Err * result)
    { return tcp.tryWrite(infos, infos_size, result); }
    inline std::size_t tryWrite(CallableTcp & tcp, char const * buffer, std::size_t size, Err * result)
    { return tcp.tryWrite(buffer, size, result); }

    inline std::size_t tryWrite(binf * infos, std::size_t infos_size, Err * result = nullptr)
    { return tryWrite(*_tcp, infos, infos_size, result); }
    inline std::size_t tryWrite(char const * buffer, std::size_t size, Err * result = nullptr)
    { return tryWrite(*_tcp, buffer, size, result); }
    // @formatter:on

public:
    virtual bool run(std::string const & ip, int port) = 0;
};

/**
 * TcpLoop class prototype.
 *
 * @author zer0
 * @date   2016-12-31
 */
class TBAG_API TcpLoop : public BaseTcpLoop
{
public:
    using WriteQueue = uv::ex::WriteQueue;

protected:
    WriteQueue _writers;

public:
    TcpLoop();
    virtual ~TcpLoop();

public:
    WriteRequest & obtainWriteRequest(Tcp & tcp);
    void releaseWriteRequest(WriteRequest & request);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPLOOP_HPP__

