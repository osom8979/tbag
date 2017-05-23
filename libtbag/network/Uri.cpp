/**
 * @file   Uri.cpp
 * @brief  Uri class implementation.
 * @author zer0
 * @date   2017-05-19
 */

#include <libtbag/network/Uri.hpp>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/log/Log.hpp>

#include <http_parser.h>
#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

Uri::Uri()
{
    // EMPTY.
}

Uri::Uri(char const * uri, bool is_connect)
{
    parse(std::string(uri), is_connect);
}

Uri::Uri(std::string const & uri, bool is_connect)
{
    parse(uri, is_connect);
}

Uri::Uri(Uri const & obj)
{
    (*this) = obj;
}

Uri::Uri(Uri && obj)
{
    (*this) = std::move(obj);
}

Uri::~Uri()
{
    // EMPTY.
}

Uri & Uri::operator =(char const * uri)
{
    parse(std::string(uri));
    return *this;
}

Uri & Uri::operator =(std::string const & uri)
{
    parse(uri);
    return *this;
}

Uri & Uri::operator =(Uri const & obj)
{
    if (this != &obj) {
        _uri      = obj._uri     ;
        _schema   = obj._schema  ;
        _host     = obj._host    ;
        _port     = obj._port    ;
        _path     = obj._path    ;
        _query    = obj._query   ;
        _fragment = obj._fragment;
        _userinfo = obj._userinfo;
    }
    return *this;
}

Uri & Uri::operator =(Uri && obj)
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

Uri::operator std::string() const
{
    return _uri;
}

Uri::operator char const * () const
{
    return _uri.c_str();
}

void Uri::swap(Uri & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _uri     .swap(obj._uri     );
        _schema  .swap(obj._schema  );
        _host    .swap(obj._host    );
        _port    .swap(obj._port    );
        _path    .swap(obj._path    );
        _query   .swap(obj._query   );
        _fragment.swap(obj._fragment);
        _userinfo.swap(obj._userinfo);
    }
}

int Uri::getPortNumber() const
{
    try {
        return std::stoi(getPort());
    } catch (...) {
        return 0;
    }
}

std::string Uri::getRequestPath() const
{
    std::string result = getPath();
    if (isQuery()) {
        result.push_back('?');
        result.append(getQuery());
    }
    if (isFragment()) {
        result.push_back('#');
        result.append(getFragment());
    }

    if (result.empty()) {
        result.push_back('/');
    }
    return result;
}

void Uri::clear()
{
    _uri     .clear();
    _schema  .clear();
    _host    .clear();
    _port    .clear();
    _path    .clear();
    _query   .clear();
    _fragment.clear();
    _userinfo.clear();
}

bool Uri::parse(std::string const & uri, bool is_connect)
{
    _uri = uri;
    if (_uri.empty()) {
        return false;
    }

    http_parser_url parser;
    ::http_parser_url_init(&parser);

    // Parse a URL; return nonzero on failure.
    if (::http_parser_parse_url(_uri.c_str(), _uri.size(), (is_connect ? 1 : 0), &parser) != 0) {
        return false;
    }

    auto updateField = [&](http_parser_url_fields field, FieldInfo & info){
        if (bitwise::checkFlag<uint16_t>(parser.field_set, field)) {
            bool enable = true;
            if (parser.field_data[field].off == 0 && parser.field_data[field].len == 0) {
                enable = false;
            }
            info.set(parser.field_data[field].off, parser.field_data[field].len, enable);
        } else {
            info.clear();
        }
    };

    updateField(UF_SCHEMA  , _schema  );
    updateField(UF_HOST    , _host    );
    updateField(UF_PORT    , _port    );
    updateField(UF_PATH    , _path    );
    updateField(UF_QUERY   , _query   );
    updateField(UF_FRAGMENT, _fragment);
    updateField(UF_USERINFO, _userinfo);
    return true;
}

Err Uri::requestAddrInfo(std::string & host, int & port, AddrFlags flags) const
{
    if (isHost() == false) {
        tDLogE("Uri::requestAddrInfo() Unknown host: {}.", _uri);
        return Err::E_ISTATE;
    }

    if (isPort() == false && isSchema() == false) {
        tDLogE("Uri::requestAddrInfo() Unknown schema or port: {}.", _uri);
        return Err::E_ISTATE;
    }

    std::string const HOST    = getHost();
    std::string const SERVICE = isSchema() ? getSchema() : std::string();

    assert(HOST.empty() == false);

    uvpp::Loop loop;
    uvpp::DnsAddrInfo addr;

    Err const REQUEST_RESULT = addr.requestAddrInfoWithSync(loop, HOST, SERVICE);
    if (REQUEST_RESULT != Err::E_SUCCESS) {
        return REQUEST_RESULT;
    }

    uvpp::DnsAddrInfo::FindFlag find_flag;
    if (flags == AddrFlags::MOST_IPV6) {
        find_flag = uvpp::DnsAddrInfo::FindFlag::MOST_IPV6;
    } else {
        find_flag = uvpp::DnsAddrInfo::FindFlag::MOST_IPV4;
    }

    auto * info = addr.findSockAddr(find_flag);
    if (info == nullptr) {
        info = addr.findFirst();
        if (info == nullptr) {
            return Err::E_ENFOUND;
        }
    }

    host = uvpp::getIpName(info);
    if (isPort()) {
        port = getPortNumber();
    } else {
        assert(SERVICE.empty() == false);
        port = uvpp::getPortNumber(info);
    }

    return Err::E_SUCCESS;
}

// ---------------
// Static methods.
// ---------------

std::string Uri::getFieldString(std::string const & original, FieldInfo const & info)
{
    return original.substr(info.offset, info.length);
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

