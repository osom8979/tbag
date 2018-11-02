/**
 * @file   Uri.cpp
 * @brief  Uri class implementation.
 * @author zer0
 * @date   2017-05-19
 */

#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/log/Log.hpp>

#include <http_parser.h>
#include <cassert>
#include <ctype.h>
#include <cstring>
#include <utility>

#define DISABLE_HTTP_PARSER_URL_FIELDS

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

int _is_triple_slash(char const * buffer, int size)
{
    if (buffer == nullptr || size == 0) {
        return _uri_state_result_error;
    }

    _uri_state s = _uri_state_schema;
    for (int i = 0; i < size; ++i) {
        switch (buffer[i]) {
        case ':':
            if (s == _uri_state_schema) {
                s = _uri_state_schema_colon;
            } else {
                return _uri_state_result_error;
            }
            break;
        case '/':
            if (s == _uri_state_schema_colon) {
                s = _uri_state_schema_slash;
            } else if (s == _uri_state_schema_slash) {
                s = _uri_state_schema_slash_slash;
            } else if (s == _uri_state_schema_slash_slash) {
                return i;
            } else {
                return _uri_state_result_error;
            }
            break;
        default:
            if (isalnum(buffer[i])) {
                if (s == _uri_state_schema) {
                    // OK.
                } else if (s == _uri_state_schema_slash_slash) {
                    return _uri_state_result_no_slash;
                } else {
                    return _uri_state_result_error;
                }
            } else {
                return _uri_state_result_error;
            }
            break;
        }
    }
    return _uri_state_result_no_slash;
}

int _is_triple_slash2(char const * buffer)
{
    if (buffer == nullptr) {
        return _uri_state_result_error;
    }
    return _is_triple_slash(buffer, strlen(buffer));
}

// -------------------
// Uri implementation.
// -------------------

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

std::string Uri::getUrl() const
{
    std::string result = getSchema();
    result.append("://");
    if (isUserinfo()) {
        result.append(getUserinfo());
        result.push_back('@');
    }
    result.append(getHost());
    if (isPort()) {
        result.push_back(':');
        result.append(getPort());
    }
    return result;
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

// ---------------
namespace __impl {
// ---------------

static bool triple_slash_parse(std::string const & uri, bool is_connect, http_parser_url & parser)
{
    // host must be present if there is a schema parsing "http:///toto" will fail.
    int const triple_slash_result = _is_triple_slash(uri.c_str(), uri.size());
    if (triple_slash_result < _uri_state_result_success_min) {
        return false;
    }

    assert(triple_slash_result >= _uri_state_result_success_min);
    char const TRIPLE_SLASH_FAKER = 'f';

    // Update host name.
    // e.g. "http:///toto" -> "http://f/toto"
    std::string const TEMP_URI = uri.substr(0, triple_slash_result) + TRIPLE_SLASH_FAKER + uri.substr(triple_slash_result);
    assert(uri.size() + 1 == TEMP_URI.size());

    ::http_parser_url_init(&parser);
    if (::http_parser_parse_url(TEMP_URI.c_str(), TEMP_URI.size(), (is_connect ? 1 : 0), &parser) != 0) {
        return false;
    }

    parser.field_data[UF_HOST].len = 0;
    parser.field_data[UF_HOST].off = 0;
    if (parser.field_data[UF_PATH].len >= 1) {
        parser.field_data[UF_PATH].off--;
    }
    if (parser.field_data[UF_QUERY].len >= 1) {
        parser.field_data[UF_QUERY].off--;
    }
    if (parser.field_data[UF_FRAGMENT].len >= 1) {
        parser.field_data[UF_FRAGMENT].off--;
    }
    return true;
}

// ------------------
} // namespace __impl
// ------------------

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
#if defined(ENABLE_TRIPLE_SLASH_FAKER)
        if (!__impl::triple_slash_parse(uri, is_connect, parser)) {
            return false;
        }
#else
        return false;
#endif
    }

    auto updateField = [&](http_parser_url_fields field, FieldInfo & info){
#if !defined(DISABLE_HTTP_PARSER_URL_FIELDS)
        if (bitwise::checkFlag<uint16_t>(parser.field_set, field))
#endif
        {
            bool enable = true;
            if (parser.field_data[field].off == 0 && parser.field_data[field].len == 0) {
                enable = false;
            }
            info.set(parser.field_data[field].off, parser.field_data[field].len, enable);
        }
#if !defined(DISABLE_HTTP_PARSER_URL_FIELDS)
        else {
            info.clear();
        }
#endif
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
        tDLogE("Uri::requestAddrInfo() Unknown host: {}", _uri);
        return Err::E_ILLSTATE;
    }

    if (isPort() == false && isSchema() == false) {
        tDLogE("Uri::requestAddrInfo() Unknown schema or port: {}", _uri);
        return Err::E_ILLSTATE;
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

// ------------------
// Extension methods.
// ------------------

std::map<std::string, std::string> Uri::getQueryMap() const
{
    std::map<std::string, std::string> result;
    for (auto & item : libtbag::string::splitTokens(getQuery(), "&")) {
        auto const key_value = libtbag::string::splitTokens(item, "=");
        switch (key_value.size()) {
        case 1:
            result.insert(std::make_pair(key_value[0], std::string())); // key only
            break;
        case 2:
            result.insert(std::make_pair(key_value[0], key_value[1])); // key & value
            break;
        default:
            // Unknown case.
            break;
        }
    }
    return result;
}

// ---------------
// Static methods.
// ---------------

std::string Uri::getFieldString(std::string const & original, FieldInfo const & info)
{
    return original.substr(info.offset, info.length);
}

// *****************************************
// ************* ERROR CHECKER *************
// *****************************************

bool __check_error__http_parser_url_fields()
{
    std::string const TEST_URI = "https://www.google.co.kr/search?q=SPS+PPS+IDR&oq=SPS+PPS+IDR+&chrome&&ie=UTF-8";
    http_parser_url parser;
    ::http_parser_url_init(&parser);
    int const code = ::http_parser_parse_url(TEST_URI.c_str(), TEST_URI.size(), 0, &parser);
    assert(code == 0);
    bool const test_result = libtbag::bitwise::checkFlag<uint16_t>(parser.field_set, UF_QUERY);
    tDLogIfA(!test_result, "__check_error__http_parser_url_fields() Not found UF_QUERY({:#b}) field: {:#b}",
             UF_QUERY, parser.field_set);
    return test_result;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

