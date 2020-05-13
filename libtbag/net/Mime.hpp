/**
 * @file   Mime.hpp
 * @brief  Mime class prototype.
 * @author zer0
 * @date   2020-05-11
 *
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types>
 * @see <https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types>
 * @see <https://www.iana.org/assignments/media-types/media-types.xhtml>
 * @see <https://tools.ietf.org/html/rfc1494>
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
#include <unordered_map>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

// Discrete types.
#define TBAG_MIME_IANA_TYPE_APPLICATION     "application"
#define TBAG_MIME_IANA_TYPE_AUDIO           "audio"
#define TBAG_MIME_IANA_TYPE_EXAMPLE         "example"
#define TBAG_MIME_IANA_TYPE_FONT            "font"
#define TBAG_MIME_IANA_TYPE_IMAGE           "image"
#define TBAG_MIME_IANA_TYPE_MODEL           "model"
#define TBAG_MIME_IANA_TYPE_TEXT            "text"
#define TBAG_MIME_IANA_TYPE_VIDEO           "video"

// Multipart types.
#define TBAG_MIME_MULTIPART_TYPE_MESSAGE      "message"
#define TBAG_MIME_MULTIPART_TYPE_MULTIPART    "multipart"

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
    TBAG_CONSTEXPR static char const * const PARAMETER_KEY_VALUE_SPLITTER = "=";
    TBAG_CONSTEXPR static char const * const ALL = "*";

    TBAG_CONSTEXPR static char const * const APPLICATION_OCTET_STREAM = TBAG_MIME_IANA_TYPE_APPLICATION "/octet-stream";
    TBAG_CONSTEXPR static char const * const APPLICATION_TBAG_BOX     = TBAG_MIME_IANA_TYPE_APPLICATION "/tbag+box";

    TBAG_CONSTEXPR static char const * const TEXT_PLAIN      = TBAG_MIME_IANA_TYPE_TEXT "/plain";
    TBAG_CONSTEXPR static char const * const TEXT_CSS        = TBAG_MIME_IANA_TYPE_TEXT "/css";
    TBAG_CONSTEXPR static char const * const TEXT_HTML       = TBAG_MIME_IANA_TYPE_TEXT "/html";
    TBAG_CONSTEXPR static char const * const TEXT_JAVASCRIPT = TBAG_MIME_IANA_TYPE_TEXT "/javascript";
    TBAG_CONSTEXPR static char const * const TEXT_CSV        = TBAG_MIME_IANA_TYPE_TEXT "/csv";

    TBAG_CONSTEXPR static char const * const IMAGE_APNG = TBAG_MIME_IANA_TYPE_IMAGE "/apng";
    TBAG_CONSTEXPR static char const * const IMAGE_BMP  = TBAG_MIME_IANA_TYPE_IMAGE "/bmp";
    TBAG_CONSTEXPR static char const * const IMAGE_GIF  = TBAG_MIME_IANA_TYPE_IMAGE "/gif";
    TBAG_CONSTEXPR static char const * const IMAGE_ICO  = TBAG_MIME_IANA_TYPE_IMAGE "/x-icon";
    TBAG_CONSTEXPR static char const * const IMAGE_JPEG = TBAG_MIME_IANA_TYPE_IMAGE "/jpeg";
    TBAG_CONSTEXPR static char const * const IMAGE_PNG  = TBAG_MIME_IANA_TYPE_IMAGE "/png";
    TBAG_CONSTEXPR static char const * const IMAGE_SVG  = TBAG_MIME_IANA_TYPE_IMAGE "/svg+xml";
    TBAG_CONSTEXPR static char const * const IMAGE_TIFF = TBAG_MIME_IANA_TYPE_IMAGE "/tiff";
    TBAG_CONSTEXPR static char const * const IMAGE_WEBP = TBAG_MIME_IANA_TYPE_IMAGE "/webp";

    TBAG_CONSTEXPR static char const * const AUDIO_WAVE     = TBAG_MIME_IANA_TYPE_AUDIO "/wave";
    TBAG_CONSTEXPR static char const * const AUDIO_WAV      = TBAG_MIME_IANA_TYPE_AUDIO "/wav";
    TBAG_CONSTEXPR static char const * const AUDIO_X_WAV    = TBAG_MIME_IANA_TYPE_AUDIO "/x-wav";
    TBAG_CONSTEXPR static char const * const AUDIO_X_PN_WAV = TBAG_MIME_IANA_TYPE_AUDIO "/x-pn-wav";
    TBAG_CONSTEXPR static char const * const AUDIO_WEBM     = TBAG_MIME_IANA_TYPE_AUDIO "/webm";
    TBAG_CONSTEXPR static char const * const AUDIO_OGG      = TBAG_MIME_IANA_TYPE_AUDIO "/ogg";

    TBAG_CONSTEXPR static char const * const VIDEO_WEBM = TBAG_MIME_IANA_TYPE_VIDEO "/webm";
    TBAG_CONSTEXPR static char const * const VIDEO_OGG  = TBAG_MIME_IANA_TYPE_VIDEO "/ogg";
    TBAG_CONSTEXPR static char const * const VIDEO_MP4  = TBAG_MIME_IANA_TYPE_VIDEO "/mp4";

    using ParamKey = std::string;
    using ParamVal = std::string;
    using Parameters = std::unordered_map<ParamKey, ParamVal>;

    std::string type;
    std::string subtype;

    Parameters parameters;

    Mime();
    Mime(std::string const & mime);
    Mime(std::string const & t, std::string const & s);
    Mime(std::string const & t, std::string const & s, Parameters const & p);
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

    // clang-format off
    inline bool isApplicationType() const { return type == TBAG_MIME_IANA_TYPE_APPLICATION   ; }
    inline bool isAudioType      () const { return type == TBAG_MIME_IANA_TYPE_AUDIO         ; }
    inline bool isExampleType    () const { return type == TBAG_MIME_IANA_TYPE_EXAMPLE       ; }
    inline bool isFontType       () const { return type == TBAG_MIME_IANA_TYPE_FONT          ; }
    inline bool isImageType      () const { return type == TBAG_MIME_IANA_TYPE_IMAGE         ; }
    inline bool isModelType      () const { return type == TBAG_MIME_IANA_TYPE_MODEL         ; }
    inline bool isTextType       () const { return type == TBAG_MIME_IANA_TYPE_TEXT          ; }
    inline bool isVideoType      () const { return type == TBAG_MIME_IANA_TYPE_VIDEO         ; }
    inline bool isMessageType    () const { return type == TBAG_MIME_MULTIPART_TYPE_MESSAGE  ; }
    inline bool isMultipartType  () const { return type == TBAG_MIME_MULTIPART_TYPE_MULTIPART; }
    // clang-format on

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

