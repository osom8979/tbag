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

HttpBuilder::HttpBuilder()
{
    // EMPTY.
}

HttpBuilder::HttpBuilder(int major, int minor)
{
    setVersion(major, minor);
}

HttpBuilder::HttpBuilder(HttpBuilder const & obj)
{
    (*this) = obj;
}

HttpBuilder::HttpBuilder(HttpBuilder && obj)
{
    (*this) = std::move(obj);
}

HttpBuilder::~HttpBuilder()
{
    // EMPTY.
}

HttpBuilder & HttpBuilder::operator =(HttpBuilder const & obj)
{
    if (this != &obj) {
        version = obj.version;
        headers = obj.headers;
        body    = obj.body   ;
        method  = obj.method ;
        uri     = obj.uri    ;
        reason  = obj.reason ;
        status  = obj.status ;
    }
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpBuilder && obj)
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

void HttpBuilder::swap(HttpBuilder & obj)
{
    version.swap(obj.version);
    headers.swap(obj.headers);
    body   .swap(obj.body   );
    method .swap(obj.method );
    uri    .swap(obj.uri    );
    reason .swap(obj.reason );
    std::swap(status, obj.status);
}

void HttpBuilder::clear()
{
    version.set(0, 0);
    headers.clear();
    body   .clear();
    method .clear();
    uri    .clear();
    reason .clear();
    status = 0;
}

HttpBuilder::String HttpBuilder::request() const
{
    return buildRequest(method, uri.getPath(), headers, body, version.major, version.minor);
}

HttpBuilder::String HttpBuilder::response() const
{
    return buildResponse(getStatus(), reason, headers, body, version.major, version.minor);
}

HttpBuilder::String HttpBuilder::buildVersionString(int major, int minor)
{
    std::stringstream ss;
    ss << HTTP << "/" << major << "." << minor;
    return ss.str();
}

HttpBuilder::String HttpBuilder::buildRequest(String const & method, String const & uri,
                                              HeaderMap const & headers, String const & body,
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

HttpBuilder::String HttpBuilder::buildResponse(String const & status, String const & reason,
                                               HeaderMap const & headers, String const & body,
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

