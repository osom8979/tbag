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
static int __global_http_on_url__             (http_parser * parser, const char * at, std::size_t length);
static int __global_http_on_status__          (http_parser * parser, const char * at, std::size_t length);
static int __global_http_on_header_field__    (http_parser * parser, const char * at, std::size_t length);
static int __global_http_on_header_value__    (http_parser * parser, const char * at, std::size_t length);
static int __global_http_on_headers_complete__(http_parser * parser);
static int __global_http_on_body__            (http_parser * parser, const char * at, std::size_t length);
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

        ::http_parser_init(&parser, HTTP_REQUEST);
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
    std::size_t execute(char const * data, std::size_t length)
    {
        return ::http_parser_execute(&parser, &settings, data, length);
    }

    int shouldKeepAlive() const
    {
        // If http_should_keep_alive() in the on_headers_complete or
        // on_message_complete callback returns 0, then this should be
        // the last message on the connection.
        // If you are the server, respond with the "Connection: close" header.
        // If you are the client, close the connection.
        return ::http_should_keep_alive(&parser);
    }

    int parseUrl(char const * buffer, std::size_t length, int is_connect)
    {
        // Parse a URL; return nonzero on failure.
        return ::http_parser_parse_url(buffer, length, is_connect, &url);
    }

    void pause(int paused)
    {
        // Pause or un-pause the parser; a nonzero value pauses.
        ::http_parser_pause(&parser, paused);
    }

    int bodyIsFinal() const
    {
        // Checks if this is the final chunk of the body.
        return ::http_body_is_final(&parser);
    }

    // ---------------
    // Static methods.
    // ---------------

    static char const * getMethodString(enum http_method m)
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
    return impl->parent->onMessageBegin();
}

int __global_http_on_url__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onUrl(at, length);
}

int __global_http_on_status__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onStatus(at, length);
}

int __global_http_on_header_field__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onHeaderField(at, length);
}

int __global_http_on_header_value__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
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

int __global_http_on_body__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
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

int HttpParser::onMessageBegin()
{
    tDLogD("HttpParser::onMessageBegin()");
    return 0;
}

int HttpParser::onUrl(const char * at, std::size_t length)
{
    tDLogD("HttpParser::onUrl({})", length);
    return 0;
}

int HttpParser::onStatus(const char * at, std::size_t length)
{
    tDLogD("HttpParser::onStatus({})", length);
    return 0;
}

int HttpParser::onHeaderField(const char * at, std::size_t length)
{
    tDLogD("HttpParser::onHeaderField({})", length);
    return 0;
}

int HttpParser::onHeaderValue(const char * at, std::size_t length)
{
    tDLogD("HttpParser::onHeaderValue({})", length);
    return 0;
}

int HttpParser::onHeadersComplete()
{
    tDLogD("HttpParser::onHeadersComplete()");
    return 0;
}

int HttpParser::onBody(const char * at, std::size_t length)
{
    tDLogD("HttpParser::onBody({})", length);
    return 0;
}

int HttpParser::onMessageComplete()
{
    tDLogD("HttpParser::onMessageComplete()");
    return 0;
}

int HttpParser::onChunkHeader()
{
    tDLogD("HttpParser::onChunkHeader()");
    return 0;
}

int HttpParser::onChunkComplete()
{
    tDLogD("HttpParser::onChunkComplete()");
    return 0;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

