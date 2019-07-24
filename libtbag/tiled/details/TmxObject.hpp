/**
 * @file   TmxObject.hpp
 * @brief  TmxObject class prototype.
 * @author zer0
 * @date   2019-07-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxEllipse.hpp>
#include <libtbag/tiled/details/TmxPolygon.hpp>
#include <libtbag/tiled/details/TmxPolyline.hpp>
#include <libtbag/tiled/details/TmxText.hpp>
#include <libtbag/tiled/details/TmxImage.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxObject class prototype.
 *
 * @author zer0
 * @date   2019-07-24
 *
 * @remarks
 *  While tile layers are very suitable for anything repetitive aligned to the tile grid,
 *  sometimes you want to annotate your map with other information,
 *  not necessarily aligned to the grid.
 *  Hence the objects have their coordinates and size in pixels,
 *  but you can still easily align that to the grid when you want to.
 *
 *  You generally use objects to add custom information to your tile map,
 *  such as spawn points, warps, exits, etc.
 *
 *  When the object has a <code>gid</code> set,
 *  then it is represented by the image of the tile with that global ID.
 *  The image alignment currently depends on the map orientation.
 *  In orthogonal orientation it’s aligned to the bottom-left
 *  while in isometric it’s aligned to the bottom-center.
 *
 *  When the object has a <code>template</code> set,
 *  it will borrow all the properties from the specified template,
 *  properties saved with the object will have higher priority,
 *  i.e. they will override the template properties.
 */
struct TBAG_API TmxObject : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "object";

    /**
     * Unique ID of the object.
     * Each object that is placed on a map gets a unique id. Even if an object was deleted,
     * no object gets the same ID.
     *
     * Can not be changed in Tiled. (since Tiled 0.11)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the object. An arbitrary string. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The type of the object. An arbitrary string. */
    TBAG_CONSTEXPR static char const * const ATT_TYPE = "type";

    /** The x coordinate of the object in pixels. */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** The y coordinate of the object in pixels. */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The width of the object in pixels (defaults to 0). */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The height of the object in pixels (defaults to 0). */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /** The rotation of the object in degrees clockwise (defaults to 0). */
    TBAG_CONSTEXPR static char const * const ATT_ROTATION = "rotation";

    /** A reference to a tile (optional). */
    TBAG_CONSTEXPR static char const * const ATT_GID = "gid";

    /** Whether the object is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    /** A reference to a template file (optional). */
    TBAG_CONSTEXPR static char const * const ATT_TEMPLATE = "template";

    int id = 0;
    std::string name;
    std::string type;
    int x = 0; // #REQUIRED
    int y = 0; // #REQUIRED
    int width = 0;
    int height = 0;
    int rotation = 0;
    int gid = 0;
    int visible = 0;
    std::string template_file;

    TmxProperties properties;
    TmxEllipse    ellipse; // since 0.9
    TmxPolygon    polygon;
    TmxPolyline   polyline;
    TmxText       text; // since 1.0
    TmxImage      image;

    TmxObject();
    TmxObject(int x_, int y_);
    ~TmxObject();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXOBJECT_HPP__

