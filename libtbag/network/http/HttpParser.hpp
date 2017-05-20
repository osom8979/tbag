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
class TBAG_API HttpParser : public HttpProperty
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

private:
    bool _message_complete;

public:
    HttpParser(Type type = Type::BOTH);
    HttpParser(HttpParser const & obj);
    HttpParser(HttpParser && obj);
    ~HttpParser();

public:
    HttpParser & operator =(HttpParser const & obj);
    HttpParser & operator =(HttpParser && obj);

public:
    // @formatter:off
    inline Type getType() const TBAG_NOEXCEPT { return TYPE; }
    inline bool isComplete() const TBAG_NOEXCEPT { return _message_complete; }
    inline void setComplete(bool flag = true) TBAG_NOEXCEPT { _message_complete = flag; }
    // @formatter:on

public:
    void clear();
    void clearCache();

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
    String getMethodName() const;
    String getErrnoName() const;
    String getErrnoDescription() const;

public:
    Size execute(char const * data, Size length);
    bool shouldKeepAlive() const;
    bool parseUrl(char const * buffer, Size length, bool is_connect = true);
    void pause(bool is_paused = true);
    bool bodyIsFinal() const TBAG_NOEXCEPT;

// Event methods.
public:
    virtual int onMessageBegin    ();
    virtual int onUrl             (String const & at);
    virtual int onStatus          (String const & at);
    virtual int onHeaderField     (String const & at);
    virtual int onHeaderValue     (String const & at);
    virtual int onHeadersComplete ();
    virtual int onBody            (String const & at);
    virtual int onMessageComplete ();
    virtual int onChunkHeader     ();
    virtual int onChunkComplete   ();
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__

