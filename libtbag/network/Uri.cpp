/**
 * @file   Uri.cpp
 * @brief  Uri class implementation.
 * @author zer0
 * @date   2017-05-19
 */

#include <libtbag/network/Uri.hpp>
#include <libtbag/bitwise/BitFlags.hpp>
#include <libtbag/log/Log.hpp>

#include <http_parser.h>
#include <cassert>
#include <utility>
#include <dep/http-parser/http_parser.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

Uri::Uri()
{
    // EMPTY.
}

Uri::Uri(String const & uri)
{
    parse(uri);
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

Uri & Uri::operator =(Uri const & obj)
{
    if (this != &obj) {
    }
    return *this;
}

Uri & Uri::operator =(Uri && obj)
{
    if (this != &obj) {
    }
    return *this;
}

bool Uri::parse(String const & uri, bool is_connect)
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
            info.set(parser.field_data[field].off, parser.field_data[field].len, true);
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

Uri::String Uri::getFieldString(String const & original, FieldInfo const & info)
{
    return original.substr(info.offset, info.offset + info.length);
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

