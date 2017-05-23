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
        url     = obj.url    ;
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
    url    .swap(obj.url    );
    reason .swap(obj.reason );
    std::swap(status, obj.status);
}

void HttpBuilder::clear()
{
    version.set(0, 0);
    headers.clear();
    body   .clear();
    method .clear();
    url    .clear();
    reason .clear();
    status = 0;
}

std::string HttpBuilder::request() const
{
    return buildRequest(method, url, headers, body, version);
}

std::string HttpBuilder::response() const
{
    return buildResponse(getStatus(), reason, headers, body, version);
}

std::string HttpBuilder::requestDefault() const
{
    return buildRequest(method, url, headers, body, version);
}

std::string HttpBuilder::responseDefault() const
{
    HeaderMap resl_headers = headers;
    if (resl_headers.find(HEADER_SERVER) == resl_headers.end()) {
        resl_headers.insert(HeaderPair(HEADER_SERVER, HEADER_DEFAULT_SERVER));
    }
    if (resl_headers.find(HEADER_CONTENT_TYPE) == resl_headers.end()) {
        resl_headers.insert(HeaderPair(HEADER_CONTENT_TYPE, HEADER_DEFAULT_CONTENT_TYPE));
    }
    if (resl_headers.find(HEADER_CONTENT_LENGTH) == resl_headers.end()) {
        resl_headers.insert(HeaderPair(HEADER_CONTENT_LENGTH, std::to_string(body.size())));
    }
    return buildResponse(getStatus(), reason, resl_headers, body, version);
}

std::string HttpBuilder::buildRequest(std::string const & method, std::string const & url,
                                      HeaderMap const & headers, std::string const & body,
                                      HttpVersion const & version)
{
    std::stringstream ss;
    ss << method << SP << url << SP << version.toString() << CRLF;
    for (auto & header : headers) {
        ss << header.first << ": " << header.second << CRLF;
    }
    ss << CRLF << body;
    return ss.str();
}

std::string HttpBuilder::buildResponse(std::string const & status, std::string const & reason,
                                       HeaderMap const & headers, std::string const & body,
                                       HttpVersion const & version)
{
    std::stringstream ss;
    ss << version.toString() << SP << status << SP << reason << CRLF;
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

