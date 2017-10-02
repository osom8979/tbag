/**
 * @file   HttpPacket.cpp
 * @brief  HttpPacket class implementation.
 * @author zer0
 * @date   2017-10-02
 */

#include <libtbag/network/http/HttpPacket.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <http_parser.h>

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

/**
 * ParserImpl class implementation.
 *
 * @author zer0
 * @date   2017-05-18
 * @date   2017-10-02 (Rename: HttpParser::HttpParserImpl -> ParserImpl::ParserImpl)
 */
struct HttpPacket::ParserImpl : private Noncopyable
{
public:
    HttpPacket * parent;

    http_parser           parser;
    http_parser_settings  settings;

    bool header_complete;
    bool body_complete;
    bool message_complete;

    struct {
        common::HttpProperty property;
        std::string header_field_temp;
    } __cache__;

    ParserImpl(HttpPacket * p, ParserType parser_type)
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

    Err execute(char const * data, std::size_t size, std::size_t * read_size = nullptr)
    {
        std::size_t const EXEC_SIZE = ::http_parser_execute(&parser, &settings, data, size);
        if (read_size != nullptr) {
            *read_size = EXEC_SIZE;
        }

        if (parser.http_errno != HPE_OK) {
            return Err::E_PARSING;
        }

        __cache__.property.atRequest().version.http_major  = parser.http_major;
        __cache__.property.atRequest().version.http_minor  = parser.http_minor;
        __cache__.property.atRequest().method = ::http_method_str(static_cast<http_method>(parser.method));
        __cache__.property.atResponse().version.http_major = parser.http_major;
        __cache__.property.atResponse().version.http_minor = parser.http_minor;
        return Err::E_SUCCESS;
    }
};

// --------------------------
// Global http-parser events.
// --------------------------

int __global_http_on_message_begin__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->clear();
    return impl->parent->onMessageBegin();
}

int __global_http_on_url__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    if (length == 0) {
        property.atRequest().uri.clear();
    } else {
        property.atRequest().uri.assign(at, at + length);
    }
    return impl->parent->onUrl(at, length);
}

int __global_http_on_status__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    if (length == 0) {
        property.atResponse().reason.clear();
    } else {
        property.atResponse().reason.assign(at, at + length);
    }
    return impl->parent->onStatus(at, length);
}

int __global_http_on_header_field__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
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
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    property.atResponse().code = impl->parser.status_code;
    if (impl->__cache__.header_field_temp.empty() == false) {
        property.atHeaders().insert(impl->__cache__.header_field_temp, std::string(at, at + length));
    }
    impl->clearCache();
    return impl->parent->onHeaderValue(at, length);
}

int __global_http_on_headers_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);

    impl->header_complete = true;
    return impl->parent->onHeadersComplete();
}

int __global_http_on_body__(http_parser * parser, const char * at, std::size_t length)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    auto & property = impl->__cache__.property;

    if (::http_body_is_final(parser) != 0) {
        // Checks if this is the final chunk of the body.
        impl->body_complete = true;
    }

    property.atBody().insert(property.atBody().begin(), at, at + length);
    return impl->parent->onBody(at, length);
}

int __global_http_on_message_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
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
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkHeader();
}

int __global_http_on_chunk_complete__(http_parser * parser)
{
    assert(parser != nullptr);
    HttpPacket::ParserImpl * impl = static_cast<HttpPacket::ParserImpl*>(parser->data);
    assert(impl != nullptr);
    assert(impl->parent != nullptr);
    return impl->parent->onChunkComplete();
}

// --------------------------
// HttpPacket implementation.
// --------------------------

HttpPacket::HttpPacket(ParserType type) : _parser(new ParserImpl(this, type))
{
    assert(static_cast<bool>(_parser));
}

HttpPacket::~HttpPacket()
{
    // EMPTY.
}

void HttpPacket::clear()
{
    _parser->clear();
    _parser->clearCache();
}

Err HttpPacket::execute(char const * data, std::size_t size, std::size_t * read_size)
{
    Err const EXEC_CODE = _parser->execute(data, size, read_size);
    if (TBAG_ERR_SUCCESS(EXEC_CODE)) {
        swap(_parser->__cache__.property);
    }
    return EXEC_CODE;
}

bool HttpPacket::shouldKeepAlive() const TBAG_NOEXCEPT
{
    // If http_should_keep_alive() in the on_headers_complete or
    // on_message_complete callback returns 0, then this should be
    // the last message on the connection.
    // If you are the server, respond with the "Connection: close" header.
    // If you are the client, close the connection.
    return ::http_should_keep_alive(&_parser->parser) != 0;
}

bool HttpPacket::bodyIsFinal() const TBAG_NOEXCEPT
{
    // Checks if this is the final chunk of the body.
    return ::http_body_is_final(&_parser->parser) != 0;
}

bool HttpPacket::isUpgrade() const TBAG_NOEXCEPT
{
    return _parser->parser.upgrade == 1 ? true : false;
}

bool HttpPacket::isFinish() const TBAG_NOEXCEPT
{
    return _parser->header_complete && (_parser->body_complete || _parser->message_complete);
}

void HttpPacket::pause(bool paused)
{
    // Pause or un-pause the parser; a nonzero value pauses.
    ::http_parser_pause(&_parser->parser, (paused ? 1 : 0));
}

char const * HttpPacket::getErrnoName() const
{
    return ::http_errno_name(static_cast<http_errno>(_parser->parser.http_errno));
}

char const * HttpPacket::getErrnoDescription() const
{
    return ::http_errno_description(static_cast<http_errno>(_parser->parser.http_errno));
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

