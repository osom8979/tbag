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
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/network/http/HttpProperty.hpp>

#include <memory>
#include <string>
#include <map>

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
class TBAG_API HttpParser : private Noncopyable
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
    ~HttpParser();

public:
    // @formatter:off
    inline Type getType() const TBAG_NOEXCEPT
    { return TYPE; }
    // @formatter:on

public:
    void clear();
    void clearCache();

    HttpHeaderMap const & atHeaders() const;
    std::string getHeader(std::string const & key) const;
    bool existsHeader(std::string const & key) const;
    bool existsHeaderValue(std::string const & key, std::string const & value, bool ignore_case = true) const;

    std::string getUrl() const;
    std::string getBody() const;
    std::string getStatus() const;

    bool isComplete() const TBAG_NOEXCEPT;

public:
    int getHttpMajor() const TBAG_NOEXCEPT;
    int getHttpMinor() const TBAG_NOEXCEPT;
    int getHttpErrno() const TBAG_NOEXCEPT;

    /** Responses only. */
    int getStatusCode() const TBAG_NOEXCEPT;

    /** Requests only. */
    int getMethod() const TBAG_NOEXCEPT;

    /**
     * Check the Upgrade header.
     *
     * @retval true
     *      Upgrade header was present and the parser has exited because of that.
     * @retval false
     *      No upgrade header present.
     *
     * @warning
     *  Should be checked when execute() returns in addition to error checking.
     */
    bool isUpgrade() const TBAG_NOEXCEPT;

public:
    std::string getMethodName() const;
    std::string getErrnoName() const;
    std::string getErrnoDescription() const;

public:
    std::size_t execute(char const * data, std::size_t length);

public:
    bool shouldKeepAlive() const TBAG_NOEXCEPT;
    bool bodyIsFinal() const TBAG_NOEXCEPT;

public:
    void pause(bool is_paused = true);

public:
    http::HttpRequest getRequest() const;
    http::HttpResponse getResponse() const;

// Event methods.
public:
    virtual int onMessageBegin    ();
    virtual int onUrl             (std::string const & at);
    virtual int onStatus          (std::string const & at);
    virtual int onHeaderField     (std::string const & at);
    virtual int onHeaderValue     (std::string const & at);
    virtual int onHeadersComplete ();
    virtual int onBody            (std::string const & at);
    virtual int onMessageComplete ();
    virtual int onChunkHeader     ();
    virtual int onChunkComplete   ();

public:
    std::string toDebugString() const;
};

/**
 * HttpRequestParser class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 */
struct HttpRequestParser : public http::HttpParser
{
    HttpRequestParser() : HttpParser(HttpParser::Type::REQUEST)
    { /* EMPTY. */ }
};

/**
 * HttpResponseParser class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 */
struct HttpResponseParser : public http::HttpParser
{
    HttpResponseParser() : HttpParser(HttpParser::Type::RESPONSE)
    { /* EMPTY. */ }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__

