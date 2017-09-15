/**
 * @file   FunctionalWsClient.hpp
 * @brief  FunctionalWsClient class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALWSCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALWSCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/http/WsClient.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * Functional web-socket client class prototype.
 *
 * @author zer0
 * @date   2017-07-10
 * @date   2017-08-07 (Rename: FunctionalWebSocketClient -> FuncWsClient)
 */
class FunctionalWsClient : public WsClient
{
public:
    using StreamType = WsClient::StreamType;

    using OnWsOpen    = std::function<void(HttpResponse const &)>;
    using OnWsMessage = std::function<void(OpCode, char const *, std::size_t)>;
    using OnWsError   = std::function<void(Err)>;
    using OnWsClose   = std::function<void(uint16_t, std::string const &)>;

private:
    OnWsOpen     _ws_open_cb;
    OnWsMessage  _ws_message_cb;
    OnWsError    _ws_error_cb;
    OnWsClose    _ws_close_cb;

public:
    FunctionalWsClient(Loop & loop, StreamType type = StreamType::TCP) : WsClient(loop, type)
    { /* EMPTY. */ }
    virtual ~FunctionalWsClient()
    { /* EMPTY. */ }

public:
    inline void setOnWsOpen   (OnWsOpen    const & cb) { _ws_open_cb    = cb; }
    inline void setOnWsMessage(OnWsMessage const & cb) { _ws_message_cb = cb; }
    inline void setOnWsError  (OnWsError   const & cb) { _ws_error_cb   = cb; }
    inline void setOnWsClose  (OnWsClose   const & cb) { _ws_close_cb   = cb; }

protected:
    virtual void onWsOpen(HttpResponse const & response) override
    {
        if (static_cast<bool>(_ws_open_cb)) {
            _ws_open_cb(response);
        }
    }

    virtual void onWsMessage(OpCode op, char const * buffer, std::size_t size) override
    {
        if (static_cast<bool>(_ws_message_cb)) {
            _ws_message_cb(op, buffer, size);
        }
    }

    virtual void onWsError(Err code) override
    {
        if (static_cast<bool>(_ws_error_cb)) {
            _ws_error_cb(code);
        }
    }

    virtual void onWsClose(uint16_t code, std::string const & reason) override
    {
        if (static_cast<bool>(_ws_close_cb)) {
            _ws_close_cb(code, reason);
        }
    }
};

using FuncWsClient = FunctionalWsClient;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALWSCLIENT_HPP__

