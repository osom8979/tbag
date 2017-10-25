/**
 * @file   HttpParser.cpp
 * @brief  HttpParser class implementation.
 * @author zer0
 * @date   2017-10-03
 */

#include <libtbag/network/http/base/HttpParser.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <http_parser.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace base    {

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

/**
 * ParserImpl class implementation.
 *
 * @author zer0
 * @date   2017-05-18
 * @date   2017-10-02 (Rename: HttpParser::HttpParserImpl -> HttpPacket::ParserImpl)
 * @date   2017-10-03 (Rename: HttpPacket::HttpParserImpl -> HttpParser::ParserImpl)
 */
struct HttpParser::ParserImpl : private Noncopyable
{
public:
    HttpParser * parent;

    http_parser           parser;
    http_parser_settings  settings;

    bool header_complete;
    bool body_complete;
    bool message_complete;

    struct {
        ParserType   direction;
        HttpProperty property;
        std::string  header_field_temp;
    } __cache__;

    ParserImpl(HttpParser * p, ParserType parser_type)
            : parent(p), header_complete(false), body_complete(false), message_complete(false)
    {
        assert(p);

        http_parser_type type;
        if (parser_type == ParserType::REQUEST) {
            type = HTTP_REQUEST;
        } else if (parser_type == ParserType::RESPONSE) {
            type = HTTP_RESPONSE;
        } else {
            type = HTTP_BOTH;
        }

        ::http_parser_init(&parser, type);
        parser.data = this;

        // Callbacks should return non-zero to indicate an error. The parser will
        // then halt execution.
        //
        // The one exception is on_headers_complete. In a HTTP_RESPONSE parser
        // returning '1' from on_headers_complete will tell the parser that it
        // should not expect a body. This is used when receiving a response to a
        // HEAD request which may contain 'Content-Length' or 'Transfer-Encoding:
        // chunked' headers that indicate the presence of a body.
        //
        // Returning `2` from on_headers_complete will tell parser that it should not
        // expect neither a body nor any futher responses on this connection. This is
        // useful for handling responses to a CONNECT request which may not contain
        // `Upgrade` or `Connection: upgrade` headers.
        //
        // http_data_cb does not return data chunks. It will be called arbitrarily
        // many times for each string. E.G. you might get 10 callbacks for "on_url"
        // each providing just a few characters more data.
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

    ~ParserImpl()
    {
        // EMPTY.
    }

    void clear()
    {
        header_complete  = false;
        body_complete    = false;
        message_complete = false;
    }

    void clearCache()
    {
        __cache__.property.clear();
        __cache__.header_field_temp.clear();
    }
};

// --------------------------
// Global http-parser events.
// --------------------------

int __global_http_on_message_begin__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->clear();
    return impl->parent->onMessageBegin();
}

int __global_http_on_url__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property  = impl->__cache__.property;
    auto & direction = impl->__cache__.direction;

    direction = HttpParser::ParserType::REQUEST;
    property.method = ::http_method_str(static_cast<http_method>(parser->method));
    if (length == 0) {
        property.path.clear();
    } else {
        property.path.assign(at, at + length);
    }
    return impl->parent->onUrl(at, length);
}

int __global_http_on_status__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property  = impl->__cache__.property;
    auto & direction = impl->__cache__.direction;

    direction = HttpParser::ParserType::RESPONSE;
    if (length == 0) {
        property.reason.clear();
    } else {
        property.reason.assign(at, at + length);
    }
    return impl->parent->onStatus(at, length);
}

int __global_http_on_header_field__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    if (length == 0) {
        impl->__cache__.header_field_temp.clear();
    } else {
        impl->__cache__.header_field_temp.assign(at, at + length);
    }
    return impl->parent->onHeaderField(at, length);
}

int __global_http_on_header_value__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    property.code = impl->parser.status_code;
    if (impl->__cache__.header_field_temp.empty() == false) {
        property.insert(impl->__cache__.header_field_temp, std::string(at, at + length));
    }
    impl->__cache__.header_field_temp.clear();
    return impl->parent->onHeaderValue(at, length);
}

int __global_http_on_headers_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    property.setVersion(parser->http_major, parser->http_minor);
    impl->header_complete = true;
    return impl->parent->onHeadersComplete();
}

int __global_http_on_body__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    if (::http_body_is_final(parser) != 0) {
        // Checks if this is the final chunk of the body.
        impl->body_complete = true;
    }

    property.body.insert(property.body.end(), at, at + length);
    return impl->parent->onBody(at, length);
}

int __global_http_on_message_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
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
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkHeader();
}

int __global_http_on_chunk_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpParser::ParserImpl * impl = static_cast<HttpParser::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkComplete();
}

// --------------------------
// HttpParser implementation.
// --------------------------

HttpParser::HttpParser(ParserType type) : _impl(new ParserImpl(this, type))
{
    assert(static_cast<bool>(_impl));
}

HttpParser::~HttpParser()
{
    // EMPTY.
}

void HttpParser::clear()
{
    _impl->clear();
    _impl->clearCache();
}

Err HttpParser::execute(char const * data, std::size_t size, std::size_t * read_size, ParserType * direction)
{
    std::size_t const EXEC_SIZE = ::http_parser_execute(&_impl->parser, &_impl->settings, data, size);
    if (_impl->parser.http_errno != HPE_OK) {
        tDLogE("HttpParser::execute() Execute {} error", getErrnoName());
        return Err::E_PARSING;
    }

    if (isFinish()) {
        swap(_impl->__cache__.property);
    }
    if (read_size != nullptr) {
        *read_size = EXEC_SIZE;
    }
    if (direction != nullptr) {
        *direction = _impl->__cache__.direction;
    }
    return Err::E_SUCCESS;
}

bool HttpParser::shouldKeepAlive() const TBAG_NOEXCEPT
{
    // If http_should_keep_alive() in the on_headers_complete or
    // on_message_complete callback returns 0, then this should be
    // the last message on the connection.
    // If you are the server, respond with the "Connection: close" header.
    // If you are the client, close the connection.
    return ::http_should_keep_alive(&_impl->parser) != 0;
}

bool HttpParser::bodyIsFinal() const TBAG_NOEXCEPT
{
    // Checks if this is the final chunk of the body.
    return ::http_body_is_final(&_impl->parser) != 0;
}

bool HttpParser::isUpgrade() const TBAG_NOEXCEPT
{
    return _impl->parser.upgrade == 1 ? true : false;
}

bool HttpParser::isFinish() const TBAG_NOEXCEPT
{
    return _impl->header_complete && (_impl->body_complete || _impl->message_complete);
}

void HttpParser::pause(bool paused)
{
    // Pause or un-pause the parser; a nonzero value pauses.
    ::http_parser_pause(&_impl->parser, (paused ? 1 : 0));
}

char const * HttpParser::getErrnoName() const
{
    return ::http_errno_name(static_cast<http_errno>(_impl->parser.http_errno));
}

char const * HttpParser::getErrnoDescription() const
{
    return ::http_errno_description(static_cast<http_errno>(_impl->parser.http_errno));
}

} // namespace base
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

