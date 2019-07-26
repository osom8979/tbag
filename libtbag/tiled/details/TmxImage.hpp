/**
 * @file   TmxImage.hpp
 * @brief  TmxImage class prototype.
 * @author zer0
 * @date   2019-07-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxData.hpp>
#include <libtbag/graphic/Color.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxImage class prototype.
 *
 * @author zer0
 * @date   2019-07-24
 */
struct TBAG_API TmxImage : protected libtbag::dom::xml::XmlHelper
{
    using Color = libtbag::graphic::Color;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "image";

    /**
     * Used for embedded images, in combination with a <code>data</code> child element.
     * Valid values are file extensions like <code>png</code>, <code>gif</code>,
     * <code>jpg</code>, <code>bmp</code> etc.
     */
    TBAG_CONSTEXPR static char const * const ATT_FORMAT = "format";

    /**
     * Used by some versions of Tiled Java.
     * Deprecated and unsupported by Tiled Qt.
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /**
     * The reference to the tileset image file
     * (Tiled supports most common image formats).
     */
    TBAG_CONSTEXPR static char const * const ATT_SOURCE = "source";

    /**
     * Defines a specific color that is treated as transparent
     * (example value: \#FF00FF for magenta).
     * Up until Tiled 0.12, this value is written out
     * without a \# but this is planned to change.
     */
    TBAG_CONSTEXPR static char const * const ATT_TRANS = "trans";

    /**
     * The image width in pixels
     * (optional, used for tile index correction when the image changes)
     */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /**
     * The image height in pixels (optional)
     */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    std::string format;
    int id;
    std::string source;
    Color trans;
    int width;
    int height;

    TmxData data;

    TmxImage();
    TmxImage(std::string const & f, int i, std::string const & s, Color t, int w, int h);
    ~TmxImage();

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGE_HPP__

