/**
 * @file   HttpVersion.hpp
 * @brief  HttpVersion class prototype.
 * @author zer0
 * @date   2017-09-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPVERSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPVERSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <algorithm>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

/**
 * Http version structure.
 *
 * @author zer0
 * @date   2017-05-20
 * @date   2017-09-29 (Rename: HttpVersionProperty -> HttpVersion)
 *
 * @remarks
 *  Use 'http_' as a prefix to avoid variable name conflicts.
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-3.1>
 */
struct HttpVersion
{
    TBAG_CONSTEXPR static char const * const VERSION_PREFIX = "HTTP";

    int http_major;
    int http_minor;

    HttpVersion(int v1 = DEFAULT_HTTP_VERSION_MAJOR, int v2 = DEFAULT_HTTP_VERSION_MINOR) TBAG_NOEXCEPT
            : http_major(v1), http_minor(v2)
    { /* EMPTY. */ }
    ~HttpVersion()
    { /* EMPTY. */ }

    inline void set(int v1, int v2) TBAG_NOEXCEPT
    {
        http_major = v1;
        http_minor = v2;
    }

    inline void setMajor(int val) TBAG_NOEXCEPT { http_major = val; }
    inline void setMinor(int val) TBAG_NOEXCEPT { http_minor = val; }

    inline int getMajor() const TBAG_NOEXCEPT { return http_major; }
    inline int getMinor() const TBAG_NOEXCEPT { return http_minor; }

    inline friend bool operator ==(HttpVersion & lh, HttpVersion & rh) TBAG_NOEXCEPT
    {
        return lh.http_major == rh.http_major && lh.http_minor == rh.http_minor;
    }

    void swap(HttpVersion & obj)
    {
        std::swap(http_major, obj.http_major);
        std::swap(http_minor, obj.http_minor);
    }

    std::string toString() const
    {
        return toString(http_major, http_minor);
    }

    /**
     * @remarks
     *  @code
     *   HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     *  @endcode
     */
    static std::string toString(int major, int minor)
    {
        return string::fformat("HTTP/{}.{}", major, minor);
    }
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPVERSION_HPP__

