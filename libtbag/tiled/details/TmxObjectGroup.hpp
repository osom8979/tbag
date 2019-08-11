/**
 * @file   TmxObjectGroup.hpp
 * @brief  TmxObjectGroup class prototype.
 * @author zer0
 * @date   2019-08-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECTGROUP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECTGROUP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxObject.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxObjectGroup class prototype.
 *
 * @author zer0
 * @date   2019-08-11
 */
struct TBAG_API TmxObjectGroup : protected libtbag::dom::xml::XmlHelper
{
    using Objects = std::vector<TmxObject>;
    using Color = libtbag::graphic::Color;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "objectgroup";

    /**
     * Unique ID of the layer.
     * Each layer that added to a map gets a unique id.
     * Even if a layer is deleted, no layer ever gets the same ID.
     * Can not be changed in Tiled. (since Tiled 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the object group. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The color used to display the objects in this group. */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /**
     * The x coordinate of the object group in tiles.
     * Defaults to 0 and can no longer be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /**
     * The y coordinate of the object group in tiles.
     * Defaults to 0 and can no longer be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The width of the object group in tiles. Meaningless. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The height of the object group in tiles. Meaningless. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    /**
     * Rendering offset for this object group in pixels.
     * Defaults to 0. (since 0.14)
     */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /**
     * Rendering offset for this object group in pixels.
     * Defaults to 0. (since 0.14)
     */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";

    /**
     * Whether the objects are drawn according to the order of appearance ("index")
     * or sorted by their y-coordinate ("topdown"). Defaults to "topdown".
     */
    TBAG_CONSTEXPR static char const * const ATT_DRAWORDER = "draworder";
    TBAG_CONSTEXPR static char const * const VAL_INDEX = "index";
    TBAG_CONSTEXPR static char const * const VAL_TOPDOWN = "topdown";

    int id = 0;
    std::string name;
    Color color;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int opacity = 1;
    int visible = 1;
    int offsetx = 0;
    int offsety = 0;

    enum DrawOrder
    {
        NONE,
        TOPDOWN,
        INDEX,
    };

    DrawOrder draworder = DrawOrder::TOPDOWN;

    TmxProperties properties;
    Objects objects;

    TmxObjectGroup();
    ~TmxObjectGroup();

    static DrawOrder getDrawOrder(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getDrawOrderName(DrawOrder order) TBAG_NOEXCEPT;

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECTGROUP_HPP__

