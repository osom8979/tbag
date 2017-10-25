/**
 * @file   HttpFilter.hpp
 * @brief  HttpFilter class prototype.
 * @author zer0
 * @date   2017-05-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPFILTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPFILTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/http/base/HttpProperty.hpp>

#include <string>
#include <regex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace base    {

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

    virtual bool filter(HttpRequest const & request) = 0;
};

/**
 * HttpBaseFilter class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 * @date   2017-10-16 (HttpDefaultFilter -> HttpBaseFilter)
 */
struct TBAG_API HttpBaseFilter : public HttpFilterInterface
{
    std::string _method;
    std::regex  _regex;

    HttpBaseFilter();
    explicit HttpBaseFilter(std::string const & method);
    explicit HttpBaseFilter(std::regex  const & regex);
    explicit HttpBaseFilter(std::string const & method, std::string const & regex);
    explicit HttpBaseFilter(std::string const & method, std::regex const & regex);
    virtual ~HttpBaseFilter();

    virtual bool filter(HttpRequest const & request) override;
};

} // namespace base

using namespace libtbag::network::http::base;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPFILTER_HPP__

