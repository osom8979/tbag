/**
 * @file   HttpFilter.hpp
 * @brief  HttpFilter class prototype.
 * @author zer0
 * @date   2017-05-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPFILTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPFILTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/http/HttpParser.hpp>

#include <string>
#include <regex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * Http filter interface.
 *
 * @author zer0
 * @date   2017-05-24
 */
struct HttpFilterInterface
{
    HttpFilterInterface() { /* EMPTY. */ }
    virtual ~HttpFilterInterface() { /* EMPTY. */ }

    virtual bool filter(http::HttpParser const & request) = 0;
};

/**
 * HttpDefaultFilter class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 */
class TBAG_API HttpDefaultFilter : public HttpFilterInterface
{
private:
    std::string _method;
    std::regex  _regex;

public:
    HttpDefaultFilter();
    HttpDefaultFilter(std::string const & method, std::string const & regex);
    HttpDefaultFilter(std::string const & method, std::regex const & regex);
    HttpDefaultFilter(std::string const & regex);
    HttpDefaultFilter(std::regex  const & regex);
    HttpDefaultFilter(HttpDefaultFilter const & obj);
    HttpDefaultFilter(HttpDefaultFilter && obj);
    virtual ~HttpDefaultFilter();

public:
    HttpDefaultFilter & operator =(HttpDefaultFilter const & obj);
    HttpDefaultFilter & operator =(HttpDefaultFilter && obj);

public:
    virtual bool filter(http::HttpParser const & request) override;
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPFILTER_HPP__

