/**
 * @file   HttpFilter.cpp
 * @brief  HttpFilter class implementation.
 * @author zer0
 * @date   2017-05-24
 */

#include <libtbag/network/http/HttpFilter.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpPathFilter::HttpPathFilter()
{
    // EMPTY.
}

HttpPathFilter::HttpPathFilter(std::string const & regex) : _regex(regex)
{
    // EMPTY.
}

HttpPathFilter::HttpPathFilter(std::regex const & regex) : _regex(regex)
{
    // EMPTY.
}

HttpPathFilter::~HttpPathFilter()
{
    // EMPTY.
}

bool HttpPathFilter::filter(HttpRequest const & request)
{
    return std::regex_match(request.url, _regex);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

