/**
 * @file   TmxMap.hpp
 * @brief  TmxMap class prototype.
 * @author zer0
 * @date   2019-08-15
 *
 * @see <https://doc.mapeditor.org/en/stable/reference/tmx-map-format/>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXMAP_HPP__

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
#include <libtbag/tiled/details/TmxTileSet.hpp>
#include <libtbag/tiled/details/TmxLayer.hpp>
#include <libtbag/tiled/details/TmxObjectGroup.hpp>
#include <libtbag/tiled/details/TmxImageLayer.hpp>
#include <libtbag/tiled/details/TmxGroup.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxMap class prototype.
 *
 * The <code>tilewidth</code> and <code>tileheight</code>
 * properties determine the general grid size of the map.
 * The individual tiles may have different sizes.
 * Larger tiles will extend at the top and right
 * (anchored to the bottom left).
 *
 * A map contains three different kinds of layers.
 * Tile layers were once the only type, and are simply called <code>layer</code>,
 * object layers have the <code>objectgroup</code> tag
 * and image layers use the <code>imagelayer</code> tag.
 * The order in which these layers appear is the order in which the layers are rendered by Tiled.
 *
 * @author zer0
 * @date   2019-08-15
 */
struct TBAG_API TmxMap : protected libtbag::dom::xml::XmlHelper
{
    using Color = libtbag::graphic::Color;
    using TileSets = std::vector<TmxTileSet>;
    using Layers = std::vector<TmxLayer>;
    using ObjectGroups = std::vector<TmxObjectGroup>;
    using ImageLayers = std::vector<TmxImageLayer>;
    using Groups = std::vector<TmxGroup>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "map";

    /**
     * The TMX format version.
     * Was "1.0" so far, and will be incremented to match minor Tiled releases.
     */
    TBAG_CONSTEXPR static char const * const ATT_VERSION = "version";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_VERSION = "1.0";

    /**
     * The Tiled version used to save the file (since Tiled 1.0.1).
     * May be a date (for snapshot builds).
     */
    TBAG_CONSTEXPR static char const * const ATT_TILEDVERSION = "tiledversion";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_TILEDVERSION = "1.0.1";

    /**
     * Map orientation.
     * Tiled supports "orthogonal", "isometric", "staggered" and "hexagonal"
     * (since 0.11).
     */
    TBAG_CONSTEXPR static char const * const ATT_ORIENTATION = "orientation";
    TBAG_CONSTEXPR static char const * const VAL_ORTHOGONAL  = "orthogonal";
    TBAG_CONSTEXPR static char const * const VAL_ISOMETRIC   = "isometric";
    TBAG_CONSTEXPR static char const * const VAL_STAGGERED   = "staggered";
    TBAG_CONSTEXPR static char const * const VAL_HEXAGONAL   = "hexagonal";

    /**
     * The order in which tiles on tile layers are rendered.
     * Valid values are right-down (the default), right-up, left-down and left-up.
     * In all cases, the map is drawn row-by-row.
     * (only supported for orthogonal maps at the moment)
     */
    TBAG_CONSTEXPR static char const * const ATT_RENDERORDER = "renderorder";
    TBAG_CONSTEXPR static char const * const VAL_RIGHT_DOWN  = "right-down";
    TBAG_CONSTEXPR static char const * const VAL_RIGHT_UP    = "right-up";
    TBAG_CONSTEXPR static char const * const VAL_LEFT_DOWN   = "left-down";
    TBAG_CONSTEXPR static char const * const VAL_LEFT_UP     = "left-up";

    /** The map width in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The map height in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /** The width of a tile. */
    TBAG_CONSTEXPR static char const * const ATT_TILEWIDTH = "tilewidth";

    /** The height of a tile. */
    TBAG_CONSTEXPR static char const * const ATT_TILEHEIGHT = "tileheight";

    /**
     * Only for hexagonal maps.
     * Determines the width or height (depending on the staggered axis)
     * of the tile's edge, in pixels.
     */
    TBAG_CONSTEXPR static char const * const ATT_HEXSIDELENGTH = "hexsidelength";

    /**
     * For staggered and hexagonal maps,
     * determines which axis ("x" or "y") is staggered.
     * (since 0.11)
     */
    TBAG_CONSTEXPR static char const * const ATT_STAGGERAXIS = "staggeraxis";
    TBAG_CONSTEXPR static char const * const VAL_X = "x";
    TBAG_CONSTEXPR static char const * const VAL_Y = "y";

    /**
     * For staggered and hexagonal maps,
     * determines whether the "even" or "odd" indexes along the staggered axis are shifted.
     * (since 0.11)
     */
    TBAG_CONSTEXPR static char const * const ATT_STAGGERINDEX = "staggerindex";
    TBAG_CONSTEXPR static char const * const VAL_EVEN = "even";
    TBAG_CONSTEXPR static char const * const VAL_ODD = "odd";

    /**
     * The background color of the map.
     * (optional, may include alpha value since 0.15 in the form \#AARRGGBB)
     */
    TBAG_CONSTEXPR static char const * const ATT_BACKGROUNDCOLOR = "backgroundcolor";

    /**
     * Stores the next available ID for new layers.
     * This number is stored to prevent reuse of the same ID after layers have been removed.
     * (since 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_NEXTLAYERID = "nextlayerid";

    /**
     * Stores the next available ID for new objects.
     * This number is stored to prevent reuse of the same ID after objects have been removed.
     * (since 0.11)
     */
    TBAG_CONSTEXPR static char const * const ATT_NEXTOBJECTID = "nextobjectid";

    /**
     * Whether the map has infinite dimensions. (Boolean)
     */
    TBAG_CONSTEXPR static char const * const ATT_INFINITE = "infinite";

    enum class Orientation
    {
        NONE,
        ORTHOGONAL,
        ISOMETRIC,
        STAGGERED,
        HEXAGONAL,
    };

    enum class RenderOrder
    {
        NONE,
        RIGHT_DOWN,
        RIGHT_UP,
        LEFT_DOWN,
        LEFT_UP,
    };

    enum class StaggerAxis
    {
        NONE, X, Y,
    };

    enum class StaggerIndex
    {
        NONE, ODD, EVEN,
    };

    std::string version;
    std::string tiled_version;
    Orientation orientation;
    RenderOrder render_order;
    int width;
    int height;
    int tile_width;
    int tile_height;
    int hex_side_length;
    StaggerAxis stagger_axis;
    StaggerIndex stagger_index;
    Color background_color;
    int next_layer_id;
    int next_object_id;
    bool infinite = false;

    TmxProperties properties;
    TileSets tilesets;
    Layers layers;
    ObjectGroups object_groups;
    ImageLayers image_layers;
    Groups groups; // (since 1.0)

    TmxMap();
    ~TmxMap();

    static Orientation getOrientation(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getOrientationName(Orientation o) TBAG_NOEXCEPT;

    static RenderOrder getRenderOrder(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getRenderOrderName(RenderOrder o) TBAG_NOEXCEPT;

    static StaggerAxis getStaggerAxis(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getStaggerAxisName(StaggerAxis a) TBAG_NOEXCEPT;

    static StaggerIndex getStaggerIndex(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getStaggerIndexName(StaggerIndex i) TBAG_NOEXCEPT;

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXMAP_HPP__

