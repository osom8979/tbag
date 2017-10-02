/**
 * @file   HttpParser.hpp
 * @brief  HttpParser class prototype.
 * @author zer0
 * @date   2017-10-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/http/common/HttpProperty.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

/**
 * HttpParser class prototype.
 *
 * @author zer0
 * @date   2017-05-18
 * @date   2017-10-02 (Rename: HttpParser -> HttpPacket)
 * @date   2017-10-03 (Rename: HttpPacket -> HttpParser)
 */
class TBAG_API HttpParser : public common::HttpProperty,
                            private Noncopyable
{
public:
    enum class ParserType
    {
        REQUEST, RESPONSE, BOTH
    };

public:
    struct ParserImpl;
    friend struct ParserImpl;

    using UniqueParserImpl = std::shared_ptr<ParserImpl>;

private:
    UniqueParserImpl _parser;

public:
    HttpParser(ParserType type = ParserType::BOTH);
    virtual ~HttpParser();

public:
    void clear();

public:
    Err execute(char const * data, std::size_t size, std::size_t * read_size = nullptr);

public:
    bool shouldKeepAlive() const TBAG_NOEXCEPT;
    bool bodyIsFinal() const TBAG_NOEXCEPT;

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
    bool isFinish() const TBAG_NOEXCEPT;

public:
    void pause(bool paused);

public:
    char const * getErrnoName() const;
    char const * getErrnoDescription() const;

// Event methods.
public:
    // @formatter:off
    virtual int onMessageBegin    () { return 0; }
    virtual int onUrl             (char const * buffer, std::size_t size) { return 0; }
    virtual int onStatus          (char const * buffer, std::size_t size) { return 0; }
    virtual int onHeaderField     (char const * buffer, std::size_t size) { return 0; }
    virtual int onHeaderValue     (char const * buffer, std::size_t size) { return 0; }
    virtual int onHeadersComplete () { return 0; }
    virtual int onBody            (char const * buffer, std::size_t size) { return 0; }
    virtual int onMessageComplete () { return 0; }
    virtual int onChunkHeader     () { return 0; }
    virtual int onChunkComplete   () { return 0; }
    // @formatter:on
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPARSER_HPP__

