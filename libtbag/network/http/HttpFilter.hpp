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
    virtual bool filter(HttpParser const & request) = 0;
};

/**
 * HttpFilter class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 */
class TBAG_API HttpPathFilter : public HttpFilterInterface
{
private:
    std::regex _regex;

public:
    HttpPathFilter();
    HttpPathFilter(std::string const & regex);
    HttpPathFilter(std::regex const & regex);
    virtual ~HttpPathFilter();

public:
    virtual bool filter(HttpParser const & request) override;
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPFILTER_HPP__

