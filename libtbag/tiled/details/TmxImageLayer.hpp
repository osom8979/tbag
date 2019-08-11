/**
 * @file   TmxImageLayer.hpp
 * @brief  TmxImageLayer class prototype.
 * @author zer0
 * @date   2019-08-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGELAYER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGELAYER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxImage.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxImageLayer class prototype.
 *
 * @author zer0
 * @date   2019-08-11
 *
 * A layer consisting of a single image.
 */
struct TBAG_API TmxImageLayer : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "imagelayer";

    /**
     * Unique ID of the layer.
     * Each layer that added to a map gets a unique id.
     * Even if a layer is deleted, no layer ever gets the same ID.
     * Can not be changed in Tiled. (since Tiled 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the image layer. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** Rendering offset of the image layer in pixels. Defaults to 0. (since 0.15) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /** Rendering offset of the image layer in pixels. Defaults to 0. (since 0.15) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";

    /** The x position of the image layer in pixels. (deprecated since 0.15) */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** The y position of the image layer in pixels. (deprecated since 0.15) */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    int id = 0;
    std::string name;
    int offsetx = 0;
    int offsety = 0;
    int x = 0;
    int y = 0;
    int opacity = 1;
    int visible = 1;

    TmxProperties properties;
    TmxImage image;

    TmxImageLayer();
    ~TmxImageLayer();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXIMAGELAYER_HPP__

