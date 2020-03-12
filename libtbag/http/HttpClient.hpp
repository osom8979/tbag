/**
 * @file   HttpClient.hpp
 * @brief  HttpClient class prototype.
 * @author zer0
 * @date   2020-03-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/ErrPair.hpp>
#include <libtbag/http/CivetWebBypass.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpClient class prototype.
 *
 * @author zer0
 * @date   2020-03-10
 */
class TBAG_API HttpClient : private Noncopyable
{
public:
    using ErrString = libtbag::ErrPair<std::string>;
    using UniqueConnection = std::unique_ptr<mg_connection, decltype(&mg_close_connection)>;

public:
    TBAG_CONSTEXPR static int const ERROR_MESSAGE_BUFFER_SIZE = 1024;
    TBAG_CONSTEXPR static int const INFINITY_TIMEOUT = -1;

private:
    UniqueConnection _conn;

public:
    HttpClient();
    virtual ~HttpClient();

public:
    ErrMsg open(std::string const & host, int port, bool use_ssl = false);
    void close();

public:
    bool isOpen() const;

public:
    Err write(std::string const & body) const;
    ErrMsg wait(int timeout_ms = INFINITY_TIMEOUT);
    bool getResponseInfo(mg_response_info * out) const;
    ErrString read() const;
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

