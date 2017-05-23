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

    using HeaderMap  = std::map<std::string, std::string>;
    using HeaderPair = HeaderMap::value_type;

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

    HeaderMap const & atHeaders() const;
    std::string getHeader(std::string const & key) const;
    bool existsHeader(std::string const & key) const;

    std::string getUrl() const;
    std::string getBody() const;
    std::string getStatus() const;

    bool isComplete() const TBAG_NOEXCEPT;

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
    std::string getMethodName() const;
    std::string getErrnoName() const;
    std::string getErrnoDescription() const;

public:
    std::size_t execute(char const * data, std::size_t length);
    bool shouldKeepAlive() const;
    void pause(bool is_paused = true);
    bool bodyIsFinal() const TBAG_NOEXCEPT;

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
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPARSER_HPP__

