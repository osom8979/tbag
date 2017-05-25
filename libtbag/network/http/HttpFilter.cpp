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

HttpDefaultFilter::HttpDefaultFilter()
{
    // EMPTY.
}

HttpDefaultFilter::HttpDefaultFilter(std::string const & method, std::string const & regex) : _method(method), _regex(regex)
{
    // EMPTY.
}

HttpDefaultFilter::HttpDefaultFilter(std::string const & method, std::regex const & regex) : _method(method), _regex(regex)
{
    // EMPTY.
}

HttpDefaultFilter::HttpDefaultFilter(std::string const & regex) : _regex(regex)
{
    // EMPTY.
}

HttpDefaultFilter::HttpDefaultFilter(std::regex const & regex) : _regex(regex)
{
    // EMPTY.
}

HttpDefaultFilter::~HttpDefaultFilter()
{
    // EMPTY.
}

bool HttpDefaultFilter::filter(HttpParser const & request)
{
    if (_method.empty()) {
        return std::regex_match(request.getUrl(), _regex);
    } else {
        return _method == request.getMethodName() && std::regex_match(request.getUrl(), _regex);
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

