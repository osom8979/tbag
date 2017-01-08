/**
 * @file   CommonTcp.hpp
 * @brief  CommonTcp class prototype.
 * @author zer0
 * @date   2017-01-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONTCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONTCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uv/ex/CallableTcp.hpp>
#include <libtbag/uv/ex/RequestQueue.hpp>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// Forward declaration.
class Loop;

/**
 * CommonTcp class prototype.
 *
 * @author zer0
 * @date   2017-01-02
 */
class TBAG_API CommonTcp : public uv::ex::CallableTcp::Callback
{
public:
    using CallableTcp = uv::ex::CallableTcp;
    using Callback    = CallableTcp::Callback;
    using SharedTcp   = std::shared_ptr<CallableTcp>;

public:
    using Parent = Callback;

public:
    using binf = uv::binf;
    using Loop = uv::Loop;

    using ConnectRequest = uv::ConnectRequest;
    using WriteRequest   = uv::WriteRequest;
    using WriteQueue     = uv::ex::WriteQueue;

    using Buffer = std::vector<char>;

protected:
    SharedTcp _tcp;

protected:
    Buffer _read_buffer;
    WriteQueue _writers;

public:
    CommonTcp();
    CommonTcp(SharedTcp tcp);
    CommonTcp(CallableTcp * tcp);
    CommonTcp(uv::Loop & loop);
    virtual ~CommonTcp();

public:
    // @formatter:off
    inline SharedTcp       & atTcp()       TBAG_NOEXCEPT { return _tcp; }
    inline SharedTcp const & atTcp() const TBAG_NOEXCEPT { return _tcp; }
    inline void setTcp(SharedTcp     tcp) { _tcp = tcp;      }
    inline void setTcp(CallableTcp * tcp) { _tcp.reset(tcp); }
    inline Buffer       & atReadBuffer()       TBAG_NOEXCEPT { return _read_buffer; }
    inline Buffer const & atReadBuffer() const TBAG_NOEXCEPT { return _read_buffer; }
    // @formatter:on

public:
    WriteRequest * obtainWriteRequest();
    void releaseWriteRequest(WriteRequest * request);

public:
    // @formatter:off
    inline void close    () { return _tcp->close    (); }
    inline bool startRead() { return _tcp->startRead(); }
    inline bool stopRead () { return _tcp->stopRead (); }

    inline std::string getSockName() { return _tcp->getSockName(); }
    inline std::string getPeerName() { return _tcp->getPeerName(); }

    inline void setUserData(void * data) TBAG_NOEXCEPT { _tcp->setUserData(data); }

    inline void       * getUserData()       TBAG_NOEXCEPT { return _tcp->getUserData(); }
    inline void const * getUserData() const TBAG_NOEXCEPT { return _tcp->getUserData(); }
    // @formatter:on

public:
    WriteRequest * asyncWrite(binf * infos, std::size_t infos_size);
    WriteRequest * asyncWrite(char const * buffer, std::size_t size);

    std::size_t tryWrite(binf * infos, std::size_t infos_size, Err * result = nullptr);
    std::size_t tryWrite(char const * buffer, std::size_t size, Err * result = nullptr);

public:
    virtual binf onAlloc(std::size_t suggested_size) override;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONTCP_HPP__

