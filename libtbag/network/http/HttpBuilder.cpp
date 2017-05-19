/**
 * @file   HttpBuilder.cpp
 * @brief  HttpBuilder class implementation.
 * @author zer0
 * @date   2017-05-19
 */

#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpBuilder::HttpBuilder() : major(1), minor(1)
{
    // EMPTY.
}

HttpBuilder::HttpBuilder(int http_major, int http_minor) : major(http_major), minor(http_minor)
{
    // EMPTY.
}

HttpBuilder::~HttpBuilder()
{
    // EMPTY.
}

HttpBuilder & HttpBuilder::clear()
{
    headers.clear();
    body.clear();
    method.clear();
    uri.clear();
    status.clear();
    reason.clear();
    return *this;
}

HttpBuilder::String HttpBuilder::buildRequest()
{
    return buildRequest(method, uri, headers, body, major, minor);
}

HttpBuilder::String HttpBuilder::buildResponse()
{
    return buildResponse(status, reason, headers, body, major, minor);
}

HttpBuilder::String HttpBuilder::buildVersionString(int major, int minor)
{
    std::stringstream ss;
    ss << HTTP << "/" << major << "." << minor;
    return ss.str();
}

HttpBuilder::String HttpBuilder::buildRequest(String const & method,
                                              String const & uri,
                                              HeaderMap const & headers,
                                              String const & body,
                                              int major, int minor)
{
    std::stringstream ss;
    ss << method << SP << uri << SP << buildVersionString(major, minor) << CRLF;
    for (auto & header : headers) {
        ss << header.first << ": " << header.second << CRLF;
    }
    ss << CRLF << body;
    return ss.str();
}

HttpBuilder::String HttpBuilder::buildResponse(String const & status,
                                               String const & reason,
                                               HeaderMap const & headers,
                                               String const & body,
                                               int major, int minor)
{
    std::stringstream ss;
    ss << buildVersionString(major, minor) << SP << status << SP << reason << CRLF;
    for (auto & header : headers) {
        ss << header.first << ": " << header.second << CRLF;
    }
    ss << CRLF << body;
    return ss.str();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

