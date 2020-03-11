/**
 * @file   CivetWebRequestHandler.hpp
 * @brief  CivetWebRequestHandler class prototype.
 * @author zer0
 * @date   2020-03-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBREQUESTHANDLER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBREQUESTHANDLER_HPP__

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
 * CivetWebRequestHandler prototype.
 *
 * @author zer0
 * @date   2020-03-11
 */
template <typename T>
struct CivetWebRequestHandler : public CivetHandler
{
    using Base = T;
    using Callback = bool(Base::*)(CivetServer*, mg_connection*);

    Base * base = nullptr;
    Callback get_cb = nullptr;
    Callback post_cb = nullptr;
    Callback head_cb = nullptr;
    Callback put_cb = nullptr;
    Callback delete_cb = nullptr;
    Callback options_cb = nullptr;
    Callback patch_cb = nullptr;

    CivetWebRequestHandler(Base * b,
                           Callback get = nullptr,
                           Callback post = nullptr,
                           Callback head = nullptr,
                           Callback put = nullptr,
                           Callback del = nullptr,
                           Callback options = nullptr,
                           Callback patch = nullptr)
            : base(b),
              get_cb(get),
              post_cb(post),
              head_cb(head),
              put_cb(put),
              delete_cb(del),
              options_cb(options),
              patch_cb(patch)
    { /* EMPTY. */ }
    virtual ~CivetWebRequestHandler()
    { /* EMPTY. */ }

    bool handleGet(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || get_cb == nullptr) {
            return CivetHandler::handleGet(server, conn);
        }
        return (base->*get_cb)(server, conn);
    }

    bool handlePost(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || post_cb == nullptr) {
            return CivetHandler::handlePost(server, conn);
        }
        return (base->*post_cb)(server, conn);
    }

    bool handleHead(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || head_cb == nullptr) {
            return CivetHandler::handleHead(server, conn);
        }
        return (base->*head_cb)(server, conn);
    }

    bool handlePut(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || put_cb == nullptr) {
            return CivetHandler::handlePut(server, conn);
        }
        return (base->*put_cb)(server, conn);
    }

    bool handleDelete(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || delete_cb == nullptr) {
            return CivetHandler::handleDelete(server, conn);
        }
        return (base->*delete_cb)(server, conn);
    }

    bool handleOptions(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || options_cb == nullptr) {
            return CivetHandler::handleOptions(server, conn);
        }
        return (base->*options_cb)(server, conn);
    }

    bool handlePatch(CivetServer * server, mg_connection * conn) override
    {
        if (base == nullptr || patch_cb == nullptr) {
            return CivetHandler::handlePatch(server, conn);
        }
        return (base->*patch_cb)(server, conn);
    }
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_CIVETWEBREQUESTHANDLER_HPP__

