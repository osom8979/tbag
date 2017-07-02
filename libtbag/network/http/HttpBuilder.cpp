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

HttpBuilder::HttpBuilder() : HttpBuilder(1, 1)
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

// @formatter:off
HttpBuilder::HttpBuilder(HttpVersionProperty  const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpCommonProperty   const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpRequestProperty  const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpResponseProperty const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpRequest          const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpResponse         const & obj) { (*this) = obj; }
HttpBuilder::HttpBuilder(HttpProperty         const & obj) { (*this) = obj; }
// @formatter:on

HttpBuilder & HttpBuilder::operator =(HttpBuilder const & obj)
{
    if (this != &obj) {
        _property.http_major = obj._property.http_major;
        _property.http_minor = obj._property.http_minor;
        _property.headers    = obj._property.headers;
        _property.body       = obj._property.body   ;
        _property.method     = obj._property.method ;
        _property.url        = obj._property.url    ;
        _property.reason     = obj._property.reason ;
        _property.status     = obj._property.status ;
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
    std::swap(_property.http_major, obj._property.http_major);
    std::swap(_property.http_minor, obj._property.http_minor);
    _property.headers.swap(obj._property.headers);
    _property.body   .swap(obj._property.body   );
    _property.method .swap(obj._property.method );
    _property.url    .swap(obj._property.url    );
    _property.reason .swap(obj._property.reason );
    std::swap(_property.status, obj._property.status);
}

void HttpBuilder::clear()
{
    _property.http_major = 0;
    _property.http_minor = 0;
    _property.headers.clear();
    _property.body.clear();
    _property.method.clear();
    _property.url.clear();
    _property.reason.clear();
    _property.status = 0;
}

HttpVersionProperty HttpBuilder::getVersionProperty() const
{
    return HttpVersionProperty(_property.http_major, _property.http_minor);
}

HttpCommonProperty HttpBuilder::getCommonProperty() const
{
    return HttpCommonProperty(_property.headers, _property.body);
}

HttpRequestProperty HttpBuilder::getRequestProperty() const
{
    return HttpRequestProperty(_property.method, _property.url);
}

HttpResponseProperty HttpBuilder::getResponseProperty() const
{
    return HttpResponseProperty(_property.status, _property.reason);
}

HttpRequest HttpBuilder::getRequest() const
{
    HttpRequest req;
    req.http_major = _property.http_major;
    req.http_minor = _property.http_minor;
    req.headers    = _property.headers;
    req.body       = _property.body;
    req.method     = _property.method;
    req.url        = _property.url;
    return req;
}

HttpResponse HttpBuilder::getResponse() const
{
    HttpResponse rsp;
    rsp.http_major = _property.http_major;
    rsp.http_minor = _property.http_minor;
    rsp.headers    = _property.headers;
    rsp.body       = _property.body;
    rsp.status     = _property.status;
    rsp.reason     = _property.reason;
    return rsp;
}

HttpProperty HttpBuilder::getProperty() const
{
    return _property;
}

HttpBuilder & HttpBuilder::operator =(HttpVersionProperty const & obj)
{
    _property.http_major = obj.http_major;
    _property.http_minor = obj.http_minor;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpCommonProperty const & obj)
{
    _property.headers = obj.headers;
    _property.body    = obj.body;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpRequestProperty const & obj)
{
    _property.method = obj.method;
    _property.url    = obj.url;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpResponseProperty const & obj)
{
    _property.status = obj.status;
    _property.reason = obj.reason;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpRequest const & obj)
{
    _property.http_major = obj.http_major;
    _property.http_minor = obj.http_minor;
    _property.headers    = obj.headers;
    _property.body       = obj.body;
    _property.method     = obj.method;
    _property.url        = obj.url;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpResponse const & obj)
{
    _property.http_major = obj.http_major;
    _property.http_minor = obj.http_minor;
    _property.headers    = obj.headers;
    _property.body       = obj.body;
    _property.status     = obj.status;
    _property.reason     = obj.reason;
    return *this;
}

HttpBuilder & HttpBuilder::operator =(HttpProperty const & obj)
{
    if (&_property != &obj) {
        _property = obj;
    }
    return *this;
}

std::string HttpBuilder::buildRequestString() const
{
    return http::buildRequestString(_property);
}

std::string HttpBuilder::buildResponseString() const
{
    return http::buildResponseString(_property);
}

std::string HttpBuilder::buildDefaultRequestString() const
{
    return http::buildDefaultRequestString(_property);
}

std::string HttpBuilder::buildDefaultResponseString() const
{
    return http::buildDefaultResponseString(_property);
}

std::string HttpBuilder::buildRequestDebugString() const
{
    return http::toDebugString(getRequest());
}

std::string HttpBuilder::buildResponseDebugString() const
{
    return http::toDebugString(getResponse());
}

// ----------------------
// Build default methods.
// ----------------------

std::string buildDefaultRequestString(HttpProperty const & req)
{
    return buildDefaultRequestString(req.method, req.url, req.headers, req.body, req.http_major, req.http_minor);
}

std::string buildDefaultResponseString(HttpProperty const & rsp)
{
    return buildDefaultResponseString(rsp.getStatus(), rsp.reason, rsp.headers, rsp.body, rsp.http_major, rsp.http_minor);
}

std::string buildDefaultRequestString(HttpRequest const & req)
{
    return buildDefaultRequestString(req.method, req.url, req.headers, req.body, req.http_major, req.http_minor);
}

std::string buildDefaultResponseString(HttpResponse const & rsp)
{
    return buildDefaultResponseString(rsp.getStatus(), rsp.reason, rsp.headers, rsp.body, rsp.http_major, rsp.http_minor);
}

std::string buildDefaultRequestString(
        std::string const & method, std::string const & url,
        HttpHeaderMap const & headers, std::string const & body,
        int major, int minor, bool logging)
{
    HttpHeaderMap update_headers = headers;
    HttpCommonProperty::insertIfNotExists(update_headers, HEADER_USER_AGENT, DEFAULT_HEADER_USER_AGENT);
    HttpCommonProperty::insertIfNotExists(update_headers, HEADER_ACCEPT, DEFAULT_HEADER_ACCEPT);

    if (body.empty() == false) {
        HttpCommonProperty::insertIfNotExists(update_headers, HEADER_CONTENT_LENGTH, std::to_string(body.size()));
    }

    if (logging) {
        if (headers.find(HEADER_HOST) == headers.end()) {
            tDLogW("buildDefaultRequestString() Not found Host header.");
        }
        for (auto & cursor : update_headers) {
            tDLogD("buildDefaultRequestString() > {}: {}", cursor.first, cursor.second);
        }
    }

    std::string const REAL_METHOD = (method.empty() ? getHttpMethodName(HttpMethod::M_GET) : method);
    std::string const REAL_URL    = (url.empty() ? "/" : url);
    int const HTTP_MAJOR = (major == 0 ? 1 : major);
    int const HTTP_MINOR = (minor == 0 ? 1 : minor);
    return buildRequestString(REAL_METHOD, REAL_URL, update_headers, body, HTTP_MAJOR, HTTP_MINOR);
}

std::string buildDefaultResponseString(
        std::string const & status, std::string const & reason,
        HttpHeaderMap const & headers, std::string const & body,
        int major, int minor, bool logging)
{
    HttpHeaderMap update_headers = headers;
    HttpCommonProperty::insertIfNotExists(update_headers, HEADER_SERVER, DEFAULT_HEADER_SERVER);
    HttpCommonProperty::insertIfNotExists(update_headers, HEADER_CONTENT_TYPE, DEFAULT_HEADER_CONTENT_TYPE);
    HttpCommonProperty::insertIfNotExists(update_headers, HEADER_CONTENT_LENGTH, std::to_string(body.size()));

    if (logging) {
        for (auto & cursor : update_headers) {
            tDLogD("buildDefaultResponseString() < {}: {}", cursor.first, cursor.second);
        }
    }

    int const HTTP_MAJOR = (major == 0 ? 1 : major);
    int const HTTP_MINOR = (minor == 0 ? 1 : minor);
    return buildResponseString(status, reason, update_headers, body, HTTP_MAJOR, HTTP_MINOR);
}

// --------------
// Build methods.
// --------------

std::string buildRequestString(HttpProperty const & req)
{
    return buildRequestString(req.method, req.url, req.headers, req.body, req.http_major, req.http_minor);
}

std::string buildResponseString(HttpProperty const & rsp)
{
    return buildResponseString(rsp.getStatus(), rsp.reason, rsp.headers, rsp.body, rsp.http_major, rsp.http_minor);
}

std::string buildRequestString(HttpRequest const & req)
{
    return buildRequestString(req.method, req.url, req.headers, req.body, req.http_major, req.http_minor);
}

std::string buildResponseString(HttpResponse const & rsp)
{
    return buildResponseString(rsp.getStatus(), rsp.reason, rsp.headers, rsp.body, rsp.http_major, rsp.http_minor);
}

std::string buildRequestString(
        std::string const & method, std::string const & url,
        HttpHeaderMap const & headers, std::string const & body,
        int major, int minor)
{
    std::stringstream ss;
    ss << method << SP << url << SP << HttpVersionProperty::getVersionString(major, minor) << CRLF;
    for (auto & header : headers) {
        ss << header.first << ": " << header.second << CRLF;
    }
    ss << CRLF << body;
    return ss.str();
}

std::string buildResponseString(
        std::string const & status, std::string const & reason,
        HttpHeaderMap const & headers, std::string const & body,
        int major, int minor)
{
    std::stringstream ss;
    ss << HttpVersionProperty::getVersionString(major, minor) << SP << status << SP << reason << CRLF;
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

