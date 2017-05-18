/**
 * @file   HttpParser.hpp
 * @brief  HttpParser class prototype.
 * @author zer0
 * @date   2017-05-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpParser class prototype.
 *
 * @author zer0
 * @date   2017-05-18
 */
class TBAG_API HttpParser : public Noncopyable
{
public:
    class HttpParserImpl;

public:
    enum class Type : int
    {
        REQUEST, RESPONSE, BOTH
    };

public:
    using UniqueHttpParser = std::unique_ptr<HttpParserImpl>;

private:
    Type const TYPE;
    UniqueHttpParser _parser;

public:
    HttpParser(Type type = Type::BOTH);
    virtual ~HttpParser();

public:
    // @formatter:off
    inline Type getType() const TBAG_NOEXCEPT
    { return TYPE; }
    // @formatter:on

public:
    int getHttpMajor() const TBAG_NOEXCEPT;
    int getHttpMinor() const TBAG_NOEXCEPT;

    /** Responses only. */
    int getStatusCode() const TBAG_NOEXCEPT;

    /** Requests only. */
    int getMethod() const TBAG_NOEXCEPT;

    int getHttpErrno() const TBAG_NOEXCEPT;
    bool isUpgrade() const TBAG_NOEXCEPT;

public:
    virtual int onMessageBegin();
    virtual int onUrl(const char * at, std::size_t length);
    virtual int onStatus(const char * at, std::size_t length);
    virtual int onHeaderField(const char * at, std::size_t length);
    virtual int onHeaderValue(const char * at, std::size_t length);
    virtual int onHeadersComplete();
    virtual int onBody(const char * at, std::size_t length);
    virtual int onMessageComplete();
    virtual int onChunkHeader();
    virtual int onChunkComplete();
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__

