/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2020-03-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/http/CivetWebBypass.hpp>
#include <libtbag/http/CivetWebServer.hpp>
#include <libtbag/http/CivetWebEventHandler.hpp>
#include <libtbag/http/CivetWebSocketEventHandler.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2020-03-10
 */
class TBAG_API HttpServer : private Noncopyable
{
private:
    // Insert member variables.

public:
    HttpServer();
    virtual ~HttpServer();
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

