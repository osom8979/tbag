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
#include <algorithm>
#include <utility>

#define DISABLE_HTTP_PARSER_VERBOSE_LOG

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
    using HeaderMap  = HttpParser::HeaderMap;
    using HeaderPair = HttpParser::HeaderPair;

public:
    struct Cache
    {
        std::string field;
    };

public:
    HttpParser * parent;

public:
    http_parser parser;
    http_parser_settings settings;

public:
    HeaderMap headers;
    std::string body;
    std::string url;
    std::string status;

    bool header_complete;
    bool body_complete;
    bool message_complete;

public:
    Cache cache;

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

        header_complete  = false;
        body_complete    = false;
        message_complete = false;
    }

    ~HttpParserImpl()
    {
        // EMPTY.
    }

public:
    void clear()
    {
        headers.clear();
        body.clear();
        url.clear();
        status.clear();

        header_complete  = false;
        body_complete    = false;
        message_complete = false;
    }

    void clearCache()
    {
        cache.field.clear();
    }

public:
    inline bool insertHeader(std::string const & key, std::string const & val)
    {
        return headers.insert(HeaderPair(key, val)).second;
    }

    inline std::string getHeader(std::string const & key) const
    {
        auto itr = headers.find(key);
        if (itr != headers.end()) {
            return itr->second;
        }
        return std::string();
    }

    inline bool eraseHeader(std::string const & key)
    {
        return headers.erase(key) == 1U;
    }

    inline bool existsHeader(std::string const & key) const
    {
        return headers.find(key) != headers.end();
    }

    inline void appendBody(std::string const & content)
    {
        body.append(content);
    }

public:
    std::size_t execute(char const * data, std::size_t length)
    {
        return ::http_parser_execute(&parser, &settings, data, length);
    }

    bool shouldKeepAlive() const TBAG_NOEXCEPT
    {
        // If http_should_keep_alive() in the on_headers_complete or
        // on_message_complete callback returns 0, then this should be
        // the last message on the connection.
        // If you are the server, respond with the "Connection: close" header.
        // If you are the client, close the connection.
        return ::http_should_keep_alive(&parser) != 0;
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

    impl->clear();
    return impl->parent->onMessageBegin();
}

int __global_http_on_url__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (length == 0) {
        impl->url.clear();
    } else {
        impl->url.assign(at, at + length);
    }
    return impl->parent->onUrl(impl->url);
}

int __global_http_on_status__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (length == 0) {
        impl->status.clear();
    } else {
        impl->status.assign(at, at + length);
    }
    return impl->parent->onStatus(impl->status);
}

int __global_http_on_header_field__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (length == 0) {
        impl->cache.field.clear();
    } else {
        impl->cache.field.assign(at, at + length);
    }
    return impl->parent->onHeaderField(impl->cache.field);
}

int __global_http_on_header_value__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    std::string const VALUE(at, at + length);
    if (impl->cache.field.empty() == false) {
        impl->insertHeader(impl->cache.field, VALUE);
    }
    impl->clearCache();
    return impl->parent->onHeaderValue(VALUE);
}

int __global_http_on_headers_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->header_complete = true;
    return impl->parent->onHeadersComplete();
}

int __global_http_on_body__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (::http_body_is_final(parser) != 0) {
        // Checks if this is the final chunk of the body.
        impl->body_complete = true;
    }

    std::string const BODY_CHUNK(at, at + length);
    impl->appendBody(BODY_CHUNK);
    return impl->parent->onBody(BODY_CHUNK);
}

int __global_http_on_message_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::HttpParserImpl * impl = static_cast<HttpParser::HttpParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (::http_body_is_final(parser) != 0) {
        // Checks if this is the final chunk of the body.
        impl->body_complete = true;
    }

    impl->message_complete = true;
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
    _parser->clear();
    _parser->clearCache();
}

void HttpParser::clearCache()
{
    _parser->clearCache();
}

HttpParser::HeaderMap const & HttpParser::atHeaders() const
{
    return _parser->headers;
}

std::string HttpParser::getHeader(std::string const & key) const
{
    return _parser->getHeader(key);
}

bool HttpParser::existsHeader(std::string const & key) const
{
    return _parser->existsHeader(key);
}

std::string HttpParser::getUrl() const
{
    return _parser->url;
}

std::string HttpParser::getBody() const
{
    return _parser->body;
}

std::string HttpParser::getStatus() const
{
    return _parser->status;
}

bool HttpParser::isComplete() const TBAG_NOEXCEPT
{
    return _parser->header_complete && (_parser->body_complete || _parser->message_complete);
}

int HttpParser::getHttpMajor() const TBAG_NOEXCEPT
{
    return _parser->parser.http_major;
}

int HttpParser::getHttpMinor() const TBAG_NOEXCEPT
{
    return _parser->parser.http_minor;
}

int HttpParser::getHttpErrno() const TBAG_NOEXCEPT
{
    return _parser->parser.http_errno;
}

int HttpParser::getStatusCode() const TBAG_NOEXCEPT
{
    return _parser->parser.status_code;
}

int HttpParser::getMethod() const TBAG_NOEXCEPT
{
    return _parser->parser.method;
}

bool HttpParser::isUpgrade() const TBAG_NOEXCEPT
{
    // 1 = Upgrade header was present and the parser has exited because of that.
    // 0 = No upgrade header present.
    // Should be checked when http_parser_execute() returns in addition to error checking.
    return _parser->parser.upgrade == 1 ? true : false;
}

std::string HttpParser::getMethodName() const
{
    return std::string(_parser->getMethodName());
}

std::string HttpParser::getErrnoName() const
{
    return std::string(_parser->getErrnoName());
}

std::string HttpParser::getErrnoDescription() const
{
    return std::string(_parser->getErrnoDescription());
}

std::size_t HttpParser::execute(char const * data, std::size_t length)
{
    return _parser->execute(data, length);
}

bool HttpParser::shouldKeepAlive() const TBAG_NOEXCEPT
{
    return _parser->shouldKeepAlive();
}

bool HttpParser::bodyIsFinal() const TBAG_NOEXCEPT
{
    return _parser->bodyIsFinal();
}

void HttpParser::pause(bool is_paused)
{
    _parser->pause(is_paused);
}

HttpRequest HttpParser::getRequest() const
{
    HttpRequest req;
    req.http_major = _parser->parser.http_major;
    req.http_minor = _parser->parser.http_minor;
    req.headers    = _parser->headers;
    req.body       = _parser->body;
    req.method     = std::string(_parser->getMethodName());
    req.url        = _parser->url;
    return req;
}

HttpResponse HttpParser::getResponse() const
{
    HttpResponse rsp;
    rsp.http_major = _parser->parser.http_major;
    rsp.http_minor = _parser->parser.http_minor;
    rsp.headers    = _parser->headers;
    rsp.body       = _parser->body;
    rsp.status     = _parser->parser.status_code;
    rsp.reason     = _parser->status;
    return rsp;
}

// --------------
// Event methods.
// --------------

#ifndef DISABLE_HTTP_PARSER_VERBOSE_LOG
#define hp_tDLogD(...) tDLogD(__VA_ARGS__)
#else
#define hp_tDLogD(...)
#endif

int HttpParser::onMessageBegin()
{
    hp_tDLogD("HttpParser::onMessageBegin()");
    return 0;
}

int HttpParser::onUrl(std::string const & at)
{
    hp_tDLogD("HttpParser::onUrl({})", at);
    return 0;
}

int HttpParser::onStatus(std::string const & at)
{
    hp_tDLogD("HttpParser::onStatus({})", at);
    return 0;
}

int HttpParser::onHeaderField(std::string const & at)
{
    hp_tDLogD("HttpParser::onHeaderField({})", at);
    return 0;
}

int HttpParser::onHeaderValue(std::string const & at)
{
    hp_tDLogD("HttpParser::onHeaderValue({})", at);
    return 0;
}

int HttpParser::onHeadersComplete()
{
    hp_tDLogD("HttpParser::onHeadersComplete()");
    return 0;
}

int HttpParser::onBody(std::string const & at)
{
    hp_tDLogD("HttpParser::onBody({})", at);
    return 0;
}

int HttpParser::onMessageComplete()
{
    hp_tDLogD("HttpParser::onMessageComplete()");
    return 0;
}

int HttpParser::onChunkHeader()
{
    hp_tDLogD("HttpParser::onChunkHeader()");
    return 0;
}

int HttpParser::onChunkComplete()
{
    hp_tDLogD("HttpParser::onChunkComplete()");
    return 0;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

