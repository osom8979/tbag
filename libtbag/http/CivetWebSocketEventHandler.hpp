/**
 * @file   CivetWebSocketEventHandler.hpp
 * @brief  CivetWebSocketEventHandler class prototype.
 * @author zer0
 * @date   2020-03-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBSOCKETEVENTHANDLER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBSOCKETEVENTHANDLER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/http/CivetWebServer.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * CivetWebSocketEventHandler class prototype.
 *
 * @author zer0
 * @date   2020-03-11
 */
template <typename T>
struct CivetWebSocketEventHandler : public CivetWebSocketHandler
{
    using Base = T;
    using ConnectionCallback = bool(Base::*)(CivetServer*, mg_connection const *);
    using ReadyStateCallback = void(Base::*)(CivetServer*, mg_connection*);
    using DataCallback = bool(Base::*)(CivetServer*, mg_connection*, int, char*, size_t);
    using CloseCallback = void(Base::*)(CivetServer*, mg_connection const *);

    Base * base = nullptr;
    ConnectionCallback connection_cb = nullptr;
    ReadyStateCallback ready_state_cb = nullptr;
    DataCallback data_cb = nullptr;
    CloseCallback close_cb = nullptr;

    CivetWebSocketEventHandler(Base * b) : base(b)
    { /* EMPTY. */ }
    virtual ~CivetWebSocketEventHandler()
    { /* EMPTY. */ }

    bool handleConnection(CivetServer * s, mg_connection const * conn) override
    {
        if (base == nullptr || connection_cb == nullptr) {
            return CivetWebSocketHandler::handleConnection(s, conn);
        }
        return (base->*connection_cb)(s, conn);
    }

    void handleReadyState(CivetServer * s, mg_connection * conn) override
    {
        if (base == nullptr || ready_state_cb == nullptr) {
            CivetWebSocketHandler::handleReadyState(s, conn);
        }
        (base->*ready_state_cb)(s, conn);
    }

    bool handleData(CivetServer * s, mg_connection * conn, int bits, char * content, size_t size) override
    {
        if (base == nullptr || data_cb == nullptr) {
            return CivetWebSocketHandler::handleData(s, conn, bits, content, size);
        }
        return (base->*data_cb)(s, conn, bits, content, size);
    }

    void handleClose(CivetServer * s, mg_connection const * conn) override
    {
        if (base == nullptr || close_cb == nullptr) {
            CivetWebSocketHandler::handleClose(s, conn);
        }
        (base->*close_cb)(s, conn);
    }
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBSOCKETEVENTHANDLER_HPP__

