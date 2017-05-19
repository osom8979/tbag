/**
 * @file   HttpParser.cpp
 * @brief  HttpParser class implementation.
 * @author zer0
 * @date   2017-05-18
 */

#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/log/Log.hpp>

#include <http_parser.h>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// @formatter:off
static int __global_http_on_message_begin__   (http_parser * parser);
static int __global_http_on_url__             (http_parser * parser, const char * at, HttpParser::Size length);
static int __global_http_on_status__          (http_parser * parser, const char * at, HttpParser::Size length);
static int __global_http_on_header_field__    (http_parser * parser, const char * at, HttpParser::Size length);
static int __global_http_on_header_value__    (http_parser * parser, const char * at, HttpParser::Size length);
static int __global_http_on_headers_complete__(http_parser * parser);
static int __global_http_on_body__            (http_parser * parser, const char * at, HttpParser::Size length);
static int __global_http_on_message_complete__(http_parser * parser);
static int __global_http_on_chunk_header__    (http_parser * parser);
static int __global_http_on_chunk_complete__  (http_parser * parser);
// @formatter:on

// ------------------------------
// HttpParserImpl implementation.
// ------------------------------

/**
 * HttpParserImpl class implementation.
 *
 * @author zer0
 * @date   2017-05-18
 */
class HttpParser::HttpParserImpl : public Noncopyable
{
public:
    HttpParser * parent;

public:
    http_parser parser;
    http_parser_url url;
    http_parser_settings settings;

private:
    struct Cache
    {
        String field;
        String value;
    };

private:
    Cache _cache;

public:
    void clearCache()
    {
        _cache.field.clear();
        _cache.value.clear();
    }

public:
    inline Cache & atCache()
    {
        return _cache;
    }

public:
    HttpParserImpl(HttpParser * p) : parent(p)
    {
        assert(p);
        http_parser_type type;
        if (parent->TYPE == Type::REQUEST) {
            type = HTTP_REQUEST;
        } else if (parent->TYPE == Type::RESPONSE) {
            type = HTTP_RESPONSE;
        } else {
            type = HTTP_BOTH;
        }

        ::http_parser_init(&parser, type);
        parser.data = this;

        ::http_parser_url_init(&url);

        ::http_parser_settings_init(&settings);
        settings.on_message_begin    = __global_http_on_message_begin__   ;
        settings.on_url              = __global_http_on_url__             ;
        settings.on_status           = __global_http_on_status__          ;
        settings.on_header_field     = __global_http_on_header_field__    ;
        settings.on_header_value     = __global_http_on_header_value__    ;
        settings.on_headers_complete = __global_http_on_headers_complete__;
        settings.on_body             = __global_http_on_body__            ;
        settings.on_message_complete = __global_http_on_message_complete__;
        settings.on_chunk_header     = __global_http_on_chunk_header__    ;
        settings.on_chunk_complete   = __global_http_on_chunk_complete__  ;
    }

    ~HttpParserImpl()
    {
        // EMPTY.
    }

public:
    Size execute(char const * data, Size length)
    {
        return ::http_parser_execute(&parser, &settings, data, length);
    }

    bool shouldKeepAlive() const
    {
        // If http_should_keep_alive() in the on_headers_complete or
        // on_message_complete callback returns 0, then this should be
        // the last message on the connection.
        // If you are the server, respond with the "Connection: close" header.
        // If you are the client, close the connection.
        return ::http_should_keep_alive(&parser) != 0;
    }

    bool parseUrl(char const * buffer, Size length, bool is_connect)
    {
        // Parse a URL; return nonzero on failure.
        return ::http_parser_parse_url(buffer, length, (is_connect ? 1 : 0), &url) == 0;
    }

    void pause(bool paused)
    {
        // Pause or un-pause the parser; a nonzero value pauses.
        ::http_parser_pause(&parser, (paused ? 1 : 0));
    }

    bool bodyIsFinal() const TBAG_NOEXCEPT
    {
        // Checks if this is the final chunk of the body.
        return ::http_body_is_final(&parser) != 0;
    }

    char const * getMethodName() const
    {
        return getMethodName(static_cast<http_method>(parser.method));
    }

    char const * getErrnoName() const
    {
        return getErrnoName(static_cast<http_errno>(parser.http_errno));
    }

    char const * getErrnoDescription() const
    {
        return getErrnoDescription(static_cast<http_errno>(parser.http_errno));
    }

    // ---------------
    // Static methods.
    // ---------------

    static char const * getMethodName(enum http_method m)
    {
        // Returns a string version of the HTTP method.
        return ::http_method_str(m);
    }

    static const char * getErrnoName(enum http_errno err)
    {
        // Return a string name of the given error.
        return ::http_errno_name(err);
    }

    static const char * getErrnoDescription(enum http_errno err)
    {
        // Return a string description of the given error.
        return ::http_errno_description(err);
    }
};

// --------------------------
// Global http-parser events.
// --------------------------

int __global_http_on_message_begin__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->parent->clear();
    return impl->parent->onMessageBegin();
}

int __global_http_on_url__(http_parser * parser, const char * at, HttpParser::Size length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->parent->setUrl(HttpParser::String(at, at + length));
    return impl->parent->onUrl(at, length);
}

int __global_http_on_status__(http_parser * parser, const char * at, HttpParser::Size length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->parent->setStatus(HttpParser::String(at, at + length));
    return impl->parent->onStatus(at, length);
}

int __global_http_on_header_field__(http_parser * parser, const char * at, HttpParser::Size length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->atCache().field = HttpParser::String(at, at + length);
    return impl->parent->onHeaderField(at, length);
}

int __global_http_on_header_value__(http_parser * parser, const char * at, HttpParser::Size length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->atCache().value = HttpParser::String(at, at + length);
    if (impl->atCache().field.empty() == false) {
        impl->parent->setHeader(impl->atCache().field, impl->atCache().value);
    }
    impl->clearCache();
    return impl->parent->onHeaderValue(at, length);
}

int __global_http_on_headers_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onHeadersComplete();
}

int __global_http_on_body__(http_parser * parser, const char * at, HttpParser::Size length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->parent->setBody(HttpParser::String(at, at + length));
    return impl->parent->onBody(at, length);
}

int __global_http_on_message_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onMessageComplete();
}

int __global_http_on_chunk_header__(http_parser * parser)
{
    // When on_chunk_header is called, the current chunk length is stored
    // in parser->content_length.
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkHeader();
}

int __global_http_on_chunk_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkComplete();
}

// --------------------------
// HttpParser implementation.
// --------------------------

HttpParser::HttpParser(Type type) : TYPE(type), _parser(new HttpParserImpl(this))
{
    assert(static_cast<bool>(_parser));
}

HttpParser::~HttpParser()
{
    // EMPTY.
}

void HttpParser::clear()
{
    _parser->clearCache();
    _headers.clear();
    _url.clear();
    _body.clear();
    _status.clear();
}

void HttpParser::clearCache()
{
    _parser->clearCache();
}

HttpParser::String HttpParser::getHeader(String const & field) const
{
    auto itr = _headers.find(field);
    if (itr != _headers.end()) {
        return itr->second;
    }
    return String();
}

bool HttpParser::setHeader(String const & field, String const & value)
{
    return _headers.insert(HeaderPair(field, value)).second;
}

bool HttpParser::eraseHeader(String const & field)
{
    return _headers.erase(field) == 1U;
}

int HttpParser::getHttpMajor() const TBAG_NOEXCEPT
{
    return _parser->parser.http_major;
}

int HttpParser::getHttpMinor() const TBAG_NOEXCEPT
{
    return _parser->parser.http_minor;
}

int HttpParser::getStatusCode() const TBAG_NOEXCEPT
{
    return _parser->parser.status_code;
}

int HttpParser::getMethod() const TBAG_NOEXCEPT
{
    return _parser->parser.method;
}

int HttpParser::getHttpErrno() const TBAG_NOEXCEPT
{
    return _parser->parser.http_errno;
}

bool HttpParser::isUpgrade() const TBAG_NOEXCEPT
{
    // 1 = Upgrade header was present and the parser has exited because of that.
    // 0 = No upgrade header present.
    // Should be checked when http_parser_execute() returns in addition to error checking.
    return _parser->parser.upgrade == 1 ? true : false;
}

HttpParser::String HttpParser::getMethodName() const
{
    return String(_parser->getMethodName());
}

HttpParser::String HttpParser::getErrnoName() const
{
    return String(_parser->getErrnoName());
}

HttpParser::String HttpParser::getErrnoDescription() const
{
    return String(_parser->getErrnoDescription());
}

HttpParser::Size HttpParser::execute(char const * data, Size length)
{
    return _parser->execute(data, length);
}

bool HttpParser::shouldKeepAlive() const
{
    return _parser->shouldKeepAlive();
}

bool HttpParser::parseUrl(char const * buffer, Size length, bool is_connect)
{
    return _parser->parseUrl(buffer, length, is_connect);
}

void HttpParser::pause(bool is_paused)
{
    _parser->pause(is_paused);
}

bool HttpParser::bodyIsFinal() const TBAG_NOEXCEPT
{
    return _parser->bodyIsFinal();
}

// --------------
// Event methods.
// --------------

int HttpParser::onMessageBegin()
{
    return 0;
}

int HttpParser::onUrl(const char * at, Size length)
{
    return 0;
}

int HttpParser::onStatus(const char * at, Size length)
{
    return 0;
}

int HttpParser::onHeaderField(const char * at, Size length)
{
    return 0;
}

int HttpParser::onHeaderValue(const char * at, Size length)
{
    return 0;
}

int HttpParser::onHeadersComplete()
{
    return 0;
}

int HttpParser::onBody(const char * at, Size length)
{
    return 0;
}

int HttpParser::onMessageComplete()
{
    return 0;
}

int HttpParser::onChunkHeader()
{
    return 0;
}

int HttpParser::onChunkComplete()
{
    return 0;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

