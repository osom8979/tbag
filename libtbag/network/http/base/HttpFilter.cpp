/**
 * @file   HttpFilter.cpp
 * @brief  HttpFilter class implementation.
 * @author zer0
 * @date   2017-05-24
 */

#include <libtbag/network/http/base/HttpFilter.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace base    {

HttpBaseFilter::HttpBaseFilter() : _method(), _regex()
{
    // EMPTY.
}

HttpBaseFilter::HttpBaseFilter(std::string const & method) : _method(method), _regex()
{
    // EMPTY.
}

HttpBaseFilter::HttpBaseFilter(std::regex const & regex) : _method(), _regex(regex)
{
    // EMPTY.
}

HttpBaseFilter::HttpBaseFilter(std::string const & method, std::string const & regex)
        : _method(method), _regex(regex)
{
    // EMPTY.
}

HttpBaseFilter::HttpBaseFilter(std::string const & method, std::regex const & regex)
        : _method(method), _regex(regex)
{
    // EMPTY.
}

HttpBaseFilter::~HttpBaseFilter()
{
    // EMPTY.
}

bool HttpBaseFilter::filter(HttpRequest const & request)
{
    if (_method.empty()) {
        return std::regex_match(request.path, _regex);
    } else {
        return _method == request.method && std::regex_match(request.path, _regex);
    }
}

} // namespace base
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

