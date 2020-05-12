/**
 * @file   Mime.hpp
 * @brief  Mime class prototype.
 * @author zer0
 * @date   2020-05-11
 *
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types>
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types>
 * @see <https://www.iana.org/assignments/media-types/media-types.xhtml>
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
#include <ostream>

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
    TBAG_CONSTEXPR static char const * const TEXT_HTML = "text/html";
    TBAG_CONSTEXPR static char const * const TEXT_CSV = "text/csv";
    TBAG_CONSTEXPR static char const * const IMAGE_JPEG = "image/jpeg";
    TBAG_CONSTEXPR static char const * const IMAGE_PNG = "image/png";
    TBAG_CONSTEXPR static char const * const AUDIO_MPEG = "audio/mpeg";
    TBAG_CONSTEXPR static char const * const AUDIO_OGG = "audio/ogg";
    TBAG_CONSTEXPR static char const * const VIDEO_MP4 = "video/mp4";
    TBAG_CONSTEXPR static char const * const APPLICATION_OCTET_STREAM = "application/octet-stream";

    std::string type;
    std::string subtype;
    std::string parameter;

    Mime();
    Mime(std::string const & mime);
    Mime(std::string const & t, std::string const & s);
    Mime(std::string const & t, std::string const & s, std::string const & p);
    Mime(Mime const & obj);
    Mime(Mime && obj) TBAG_NOEXCEPT;
    ~Mime();

    Mime & operator =(Mime const & obj);
    Mime & operator =(Mime && obj) TBAG_NOEXCEPT;

    void swap(Mime & obj) TBAG_NOEXCEPT;
    friend void swap(Mime & lh, Mime & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

    bool isCompatible(Mime const & obj) const;

    bool operator ==(Mime const & obj) const;
    bool operator !=(Mime const & obj) const;

    void fromString(std::string const & mime);
    std::string toString() const;

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, Mime const & mime)
    {
        return os << mime.toString();
    }
};

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_MIME_HPP__

