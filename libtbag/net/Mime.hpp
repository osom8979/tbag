/**
 * @file   Mime.hpp
 * @brief  Mime class prototype.
 * @author zer0
 * @date   2020-05-11
 *
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types>
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NET_MIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NET_MIME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

/**
 * Mime class prototype.
 *
 * @author zer0
 * @date   2020-05-11
 */
struct TBAG_API Mime
{
    TBAG_CONSTEXPR static char const * const TYPE_DELIMITER = "/";
    TBAG_CONSTEXPR static char const * const PARAMETER_DELIMITER = ";";
    TBAG_CONSTEXPR static char const * const ALL = "*";

    TBAG_CONSTEXPR static char const * const TEXT_PLAIN = "text/plain";
    TBAG_CONSTEXPR static char const * const TEXT_html = "text/html";
    TBAG_CONSTEXPR static char const * const IMAGE_JPEG = "image/jpeg";
    TBAG_CONSTEXPR static char const * const IMAGE_PNG = "image/png";
    TBAG_CONSTEXPR static char const * const AUDIO_MPEG = "audio/mpeg";
    TBAG_CONSTEXPR static char const * const AUDIO_OGG = "audio/ogg";
    TBAG_CONSTEXPR static char const * const VIDEO_MP4 = "video/mp4";
    TBAG_CONSTEXPR static char const * const APPLICATION_OCTET_STREAM = "application/octet-stream";

    std::string type;
    std::string subtype;
    std::string parameter;

    std::string toString() const;
};

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_MIME_HPP__

