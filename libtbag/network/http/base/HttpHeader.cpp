/**
 * @file   HttpHeader.cpp
 * @brief  HttpHeader class implementation.
 * @author zer0
 * @date   2017-09-29
 */

#include <libtbag/network/http/base/HttpHeader.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace base    {

HttpHeader::HttpHeader() : _headers()
{
    // EMPTY.
}

HttpHeader::HttpHeader(HttpHeader const & obj)
{
    (*this) = obj;
}

HttpHeader::HttpHeader(HttpHeader && obj)
{
    (*this) = std::move(obj);
}

HttpHeader::HttpHeader(std::initializer_list<value_type> list) : _headers(list)
{
    // EMPTY.
}

HttpHeader::~HttpHeader()
{
    // EMPTY.
}

HttpHeader & HttpHeader::operator =(HttpHeader const & obj)
{
    if (this != &obj) {
        _headers = obj._headers;
    }
    return *this;
}

HttpHeader & HttpHeader::operator =(HttpHeader && obj)
{
    if (this != &obj) {
        _headers.swap(obj._headers);
    }
    return *this;
}

HttpHeader::mapped_type HttpHeader::operator [](key_type const & key)
{
    return get(key);
}

HttpHeader HttpHeader::getRegularization() const
{
    HttpHeader result;
    for (auto & cursor : _headers) {
        result.insert(value_type(string::lower(string::trim(cursor.first)),
                                 string::lower(string::trim(cursor.second))));
    }
    return result;
}

void HttpHeader::regularization()
{
    (*this) = getRegularization();
}

HttpHeader::mapped_type HttpHeader::get(key_type const & key) const
{
    if (key.empty() == false) {
        auto itr = _headers.find(key);
        if (itr != _headers.end()) {
            return itr->second;
        }
    }
    return mapped_type();
}

HttpHeader::mapped_type HttpHeader::getIgnoreCase(key_type const & key) const
{
    key_type compare_key = string::lower(key);
    key_type cursor_key;
    for (auto & cursor : _headers) {
        cursor_key = cursor.first;
        string::lowerSelf(cursor_key);
        if (cursor_key == compare_key) {
            return cursor.second;
        }
    }
    return mapped_type();
}

HttpHeader::iterator HttpHeader::insert(key_type const & key, mapped_type const & val)
{
    return _headers.insert(value_type(key, val));
}

HttpHeader::size_type HttpHeader::insert(HttpHeader const & header)
{
    size_type insert_count = 0;
    for (auto & cursor : header) {
        if (_headers.insert(cursor) != _headers.end()) {
            ++insert_count;
        }
    }
    return insert_count;
}

HttpHeader::iterator HttpHeader::insertIfNotExists(key_type const & key, mapped_type const & val)
{
    if (exists(key) == false) {
        return _headers.insert(value_type(key, val));
    }
    return _headers.end();
}

HttpHeader::size_type HttpHeader::insertIfNotExists(HttpHeader const & header)
{
    size_type insert_count = 0;
    for (auto & cursor : header) {
        if (insertIfNotExists(cursor.first, cursor.second) != _headers.end()) {
            ++insert_count;
        }
    }
    return insert_count;
}

bool HttpHeader::exists(key_type const & key) const
{
    return _headers.find(key) != _headers.end();
}

bool HttpHeader::exists(key_type const & key, mapped_type const & val) const
{
    auto pair = _headers.equal_range(key);
    for (; pair.first != pair.second; ++pair.first) {
        if (pair.first->second == val) {
            return true;
        }
    }
    return false;
}

bool HttpHeader::existsInSplitValues(key_type const & key, mapped_type const & val) const
{
    return existsInSplitValues(key, val, mapped_type(DEFAULT_VALUE_DELIMITER));
}

bool HttpHeader::existsInSplitValues(key_type const & key,
                                     mapped_type const & val,
                                     mapped_type const & delimiter) const
{
    auto pair = _headers.equal_range(key);
    std::vector<std::string> tokens;
    for (; pair.first != pair.second; ++pair.first) {
        tokens = string::splitTokens(pair.first->second, delimiter);
        for (auto & cursor : tokens) {
            if (string::trim(cursor) == val) {
                return true;
            }
        }
    }
    return false;
}

std::string HttpHeader::toMessageHeader() const
{
    std::stringstream ss;
    for (auto & header : _headers) {
        ss << string::fformat("{}: {}\r\n", header.first, header.second);
    }
    return ss.str();
}

} // namespace base
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

