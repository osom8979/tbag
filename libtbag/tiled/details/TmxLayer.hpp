/**
 * @file   TmxLayer.hpp
 * @brief  TmxLayer class prototype.
 * @author zer0
 * @date   2019-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXLAYER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXLAYER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxData.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxLayer class prototype.
 *
 * DTD:
 * @code{.dtd}
 *  <!ELEMENT layer (properties?, data)>
 *  <!ATTLIST layer
 *    name        CDATA   #REQUIRED
 *    width       CDATA   #REQUIRED
 *    height      CDATA   #REQUIRED
 *    x           CDATA   #IMPLIED
 *    y           CDATA   #IMPLIED
 *    opacity     CDATA   #IMPLIED
 *    visible     (0 | 1) #IMPLIED
 *  >
 * @endcode
 *
 * @author zer0
 * @date   2019-07-14
 *
 * @remarks
 *  All <code>tileset</code> tags shall occur before the first <code>layer</code> tag
 *  so that parsers may rely on having the tilesets before needing to resolve tiles.
 */
struct TBAG_API TmxLayer : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "layer";

    /**
     * Unique ID of the layer.
     * Each layer that added to a map gets a unique id.
     * Even if a layer is deleted, no layer ever gets the same ID.
     * Can not be changed in Tiled.
     * (since Tiled 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the layer. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /**
     * The x coordinate of the layer in tiles.
     * Defaults to 0 and can not be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_X = 0;

    /**
     * The y coordinate of the layer in tiles.
     * Defaults to 0 and can not be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_Y = 0;

    /**
     * The width of the layer in tiles.
     * Always the same as the map width for fixed-size maps.
     */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /**
     * The height of the layer in tiles.
     * Always the same as the map height for fixed-size maps.
     */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /**
     * The opacity of the layer as a value from 0 to 1.
     * Defaults to 1.
     */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_OPACITY = 1;

    /**
     * Whether the layer is shown (1) or hidden (0).
     * Defaults to 1.
     */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_VISIBLE = 1;

    /**
     * Rendering offset for this layer in pixels.
     * Defaults to 0. (since 0.14)
     */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_OFFSETX = 0;

    /**
     * Rendering offset for this layer in pixels.
     * Defaults to 0. (since 0.14)
     */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_OFFSETY = 0;

    int id;
    std::string name; // #REQUIRED
    int x;
    int y;
    int width; // #REQUIRED
    int height; // #REQUIRED
    int opacity;
    int visible;
    int offsetx;
    int offsety;

    TmxProperties properties;
    TmxData data;

    TmxLayer();
    TmxLayer(int i, std::string const & n, int x_, int y_, int w, int h, int o, int v, int ox, int oy);
    ~TmxLayer();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXLAYER_HPP__

