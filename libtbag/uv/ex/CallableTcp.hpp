/**
 * @file   CallableTcp.hpp
 * @brief  CallableTcp class prototype.
 * @author zer0
 * @date   2016-12-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLETCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLETCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Tcp.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {
namespace ex {

/**
 * CallableTcp class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
class CallableTcp : public Tcp
{
public:
    // @formatter:off
    struct Callback
    {
        // Event of Tcp.
        virtual void onConnect(ConnectRequest & request, Err code) { /* EMPTY. */ }

        // Event of Stream.
        virtual void onShutdown(ShutdownRequest & request, Err code)         { /* EMPTY. */ }
        virtual void onConnection(Err code)                                  { /* EMPTY. */ }
        virtual binf onAlloc(std::size_t suggested_size)                     { /* EMPTY. */ }
        virtual void onRead(Err code, char const * buffer, std::size_t size) { /* EMPTY. */ }
        virtual void onWrite(WriteRequest & request, Err code)               { /* EMPTY. */ }

        // Event of Handle.
        virtual void onClose()          { /* EMPTY. */ }
        virtual void onWalk(void * arg) { /* EMPTY. */ }
    };
    // @formatter:on

private:
    Callback * _cb;

public:
    // @formatter:off
    CallableTcp(Loop & l, Callback * c = nullptr) : Tcp(l), _cb(c)
    { /* EMPTY. */ }
    ~CallableTcp()
    { /* EMPTY. */ }
    // @formatter:on

public:
    // @formatter:off
    virtual void onConnect(ConnectRequest & request, Err code) override
    { if (_cb != nullptr) { _cb->onConnect(request, code); } }

    virtual void onShutdown(ShutdownRequest & request, Err code) override
    { if (_cb != nullptr) { _cb->onShutdown(request, code); } }
    virtual void onConnection(Err code) override
    { if (_cb != nullptr) { _cb->onConnection(code); } }
    virtual binf onAlloc(std::size_t suggested_size) override
    { if (_cb != nullptr) { return _cb->onAlloc(suggested_size); } return binf(); }
    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    { if (_cb != nullptr) { _cb->onRead(code, buffer, size); } }
    virtual void onWrite(WriteRequest & request, Err code) override
    { if (_cb != nullptr) { _cb->onWrite(request, code); } }

    virtual void onClose() override
    { if (_cb != nullptr) { _cb->onClose(); } }
    virtual void onWalk(void * arg) override
    { if (_cb != nullptr) { _cb->onWalk(arg); } }
    // @formatter:on
};

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_CALLABLETCP_HPP__

