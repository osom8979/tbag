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
    using String = std::string;
    using Size   = std::size_t;

    using HeaderMap = std::map<String, String>;
    using HeaderPair = HeaderMap::value_type;

private:
    Type const TYPE;
    UniqueHttpParser _parser;

private:
    HeaderMap _headers;
    String    _status;
    String    _url;
    String    _body;

private:
    struct Cache
    {
        String field;
        String value;
    };

private:
    Cache _cache;

public:
    HttpParser(Type type = Type::BOTH);
    virtual ~HttpParser();

public:
    // @formatter:off
    inline Type getType() const TBAG_NOEXCEPT { return TYPE; }
    inline Size getHeaderSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_headers.size())) { return _headers.size(); }
    inline bool isHeaderEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_headers.empty())) { return _headers.empty(); }
    inline String getStatus() const { return _status; }
    inline String getUrl() const { return _url; }
    inline String getBody() const { return _body; }
    inline void setStatus(String const & status) { _status = status; }
    inline void setUrl(String const & url) { _url = url; }
    inline void setBody(String const & body) { _body = body; }
    // @formatter:on

public:
    void clear();
    void clearCache();

public:
    String getHeader(String const & field) const;
    bool setHeader(String const & field, String const & value);
    bool eraseHeader(String const & field);

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

public:
    inline bool isComplete() const TBAG_NOEXCEPT
    { return bodyIsFinal(); }

// Event methods.
public:
    virtual int onMessageBegin();
    virtual int onUrl(const char * at, Size length);
    virtual int onStatus(const char * at, Size length);
    virtual int onHeaderField(const char * at, Size length);
    virtual int onHeaderValue(const char * at, Size length);
    virtual int onHeadersComplete();
    virtual int onBody(const char * at, Size length);
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

