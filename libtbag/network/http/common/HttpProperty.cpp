/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-09-30
 */

#include <libtbag/network/http/common/HttpProperty.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

HttpProperty::HttpProperty()
{
    // EMPTY.
}

HttpProperty::HttpProperty(HttpProperty const & obj)
{
    (*this) = obj;
}

HttpProperty::HttpProperty(HttpProperty && obj)
{
    (*this) = std::move(obj);
}

HttpProperty::~HttpProperty()
{
    // EMPTY.
}

HttpProperty & HttpProperty::operator =(HttpProperty const & obj)
{
    if (this != &obj) {
        _request  = obj._request;
        _response = obj._response;
        _headers  = obj._headers;
        _body     = obj._body;
    }
    return *this;
}

HttpProperty & HttpProperty::operator =(HttpProperty && obj)
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

void HttpProperty::swap(HttpProperty & obj)
{
    _request.swap(obj._request);
    _response.swap(obj._response);
    _headers.swap(obj._headers);
    _body.swap(obj._body);
}

std::string HttpProperty::toRequestString() const
{
    std::stringstream ss;
    ss << _request.toString() << CRLF
       << _headers.toString() << CRLF
       << _body;
    return ss.str();
}

std::string HttpProperty::toResponseString() const
{
    std::stringstream ss;
    ss << _response.toString() << CRLF
       << _headers.toString() << CRLF
       << _body;
    return ss.str();
}

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

