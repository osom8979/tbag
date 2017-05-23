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
    setVersion(1, 1);
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
        maj     = obj.maj;
        min     = obj.min;
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
    std::swap(maj, obj.maj);
    std::swap(min, obj.min);
    headers.swap(obj.headers);
    body   .swap(obj.body   );
    method .swap(obj.method );
    url    .swap(obj.url    );
    reason .swap(obj.reason );
    std::swap(status, obj.status);
}

void HttpBuilder::clear()
{
    maj = 0;
    min = 0;
    headers.clear();
    body.clear();
    method.clear();
    url.clear();
    reason.clear();
    status = 0;
}

std::string HttpBuilder::toRequestString() const
{
    return getRequestString(method, url, headers, body, maj, min);
}

std::string HttpBuilder::toResponseString() const
{
    return getResponseString(getStatus(), reason, headers, body, maj, min);
}

std::string HttpBuilder::toRequestDefaultString() const
{
    HeaderMap temp = headers;
    int http_major = (maj == 0 ? 1 : maj);
    int http_minor = (min == 0 ? 1 : min);

    Uri const URI(url);

    std::string real_method = (method.empty() ? METHOD_GET : method);
    std::string real_url    = URI.getRequestPath();

    existsOrInsert(temp, HEADER_HOST, URI.getHost());
    existsOrInsert(temp, HEADER_USER_AGENT, HEADER_DEFAULT_USER_AGENT);
    existsOrInsert(temp, HEADER_ACCEPT, HEADER_DEFAULT_ACCEPT);

    return getRequestString(real_method, real_url, temp, body, http_major, http_minor);
}

std::string HttpBuilder::toResponseDefaultString() const
{
    HeaderMap temp = headers;
    int http_major = (maj == 0 ? 1 : maj);
    int http_minor = (min == 0 ? 1 : min);

    existsOrInsert(temp, HEADER_SERVER, HEADER_DEFAULT_SERVER);
    existsOrInsert(temp, HEADER_CONTENT_TYPE, HEADER_DEFAULT_CONTENT_TYPE);
    existsOrInsert(temp, HEADER_CONTENT_LENGTH, std::to_string(body.size()));

    return getResponseString(getStatus(), reason, temp, body, http_major, http_minor);
}

void HttpBuilder::existsOrInsert(HeaderMap & headers, std::string const & key, std::string const & val)
{
    if (headers.find(key) == headers.end()) {
        headers.insert(HeaderPair(key, val));
    }
}

std::string HttpBuilder::getVersionString(int major, int minor)
{
    std::stringstream ss;
    ss << HTTP << '/' << major << '.' << minor;
    return ss.str();
}

std::string HttpBuilder::getRequestString(std::string const & method, std::string const & url,
                                          HeaderMap const & headers, std::string const & body,
                                          int major, int minor)
{
    std::stringstream ss;
    ss << method << SP << url << SP << getVersionString(major, minor) << CRLF;
    for (auto & header : headers) {
        ss << header.first << ": " << header.second << CRLF;
    }
    ss << CRLF << body;
    return ss.str();
}

std::string HttpBuilder::getResponseString(std::string const & status, std::string const & reason,
                                           HeaderMap const & headers, std::string const & body,
                                           int major, int minor)
{
    std::stringstream ss;
    ss << getVersionString(major, minor) << SP << status << SP << reason << CRLF;
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

