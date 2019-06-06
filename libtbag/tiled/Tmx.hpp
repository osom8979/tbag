/**
 * @file   Tmx.hpp
 * @brief  Tmx class prototype.
 * @author zer0
 * @date   2019-05-29
 *
 * @see <https://doc.mapeditor.org/en/stable/reference/tmx-map-format/>
 * @see <https://doc.mapeditor.org/en/stable/reference/json-map-format/>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

struct Property
{
    /** The name of the property. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /**
     * The type of the property.
     * Can be string (default), int, float, bool, color or file
     * (since 0.16, with color and file added in 0.17).
     */
    TBAG_CONSTEXPR static char const * const ATT_TYPE = "type";

    /** The value of the property. */
    TBAG_CONSTEXPR static char const * const ATT_VALUE = "value";

    std::string name;
    std::string type;
    std::string value;
};

using Properties = std::vector<Property>;

struct Tileset
{
    /**
     * The first global tile ID of this tileset
     * (this global ID maps to the first tile in this tileset).
     */
    TBAG_CONSTEXPR static char const * const ATT_FIRSTGID = "firstgid";

    /**
     * If this tileset is stored in an external TSX (Tile Set XML) file,
     * this attribute refers to that file.
     * That TSX file has the same structure as the <tileset> element described here.
     *
     * (There is the firstgid attribute missing and this source attribute is also not there.
     * These two attributes are kept in the TMX map, since they are map specific.)
     */
    TBAG_CONSTEXPR static char const * const ATT_SOURCE = "source";

    /** The name of this tileset. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The (maximum) width of the tiles in this tileset. */
    TBAG_CONSTEXPR static char const * const ATT_TILEWIDTH = "tilewidth";

    /** The (maximum) height of the tiles in this tileset. */
    TBAG_CONSTEXPR static char const * const ATT_TILEHEIGHT = "tileheight";

    /**
     * The spacing in pixels between the tiles in this tileset
     * (applies to the tileset image).
     */
    TBAG_CONSTEXPR static char const * const ATT_SPACING = "spacing";

    /**
     * The margin around the tiles in this tileset
     * (applies to the tileset image).
     */
    TBAG_CONSTEXPR static char const * const ATT_MARGIN = "margin";

    /** The number of tiles in this tileset (since 0.13) */
    TBAG_CONSTEXPR static char const * const ATT_TILECOUNT = "tilecount";

    /**
     * The number of tile columns in the tileset.
     * For image collection tilesets it is editable and is used when displaying the tileset.
     * (since 0.15)
     */
    TBAG_CONSTEXPR static char const * const ATT_COLUMNS = "columns";

    //grid 	object 	See <grid> (optional)
    //image 	string 	Image used for tiles in this set
    //imagewidth 	int 	Width of source image in pixels
    //imageheight 	int 	Height of source image in pixels
    //terrains 	array 	Array of Terrains (optional)
    //tileoffset 	object 	See <tileoffset> (optional)
    //tiles 	array 	Array of Tiles (optional)
    //transparentcolor 	string 	Hex-formatted color (#RRGGBB) (optional)
    //type 	string 	tileset (for tileset files, since 1.0)
    //wangsets 	array 	Array of Wang sets (since 1.1.5)

    int firstgid = 0;
    std::string name;
    std::string source;
    int tile_width = 0;
    int tile_height = 0;
    int spacing = 0;
    int margin = 0;
    int tile_count = 0;
    int columns = 0;

    Properties properties;
};

struct TileOffset
{
    /** Horizontal offset in pixels */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** Vertical offset in pixels (positive is down) */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";
};

struct Grid
{
    /** Orientation of the grid for the tiles in this tileset (orthogonal or isometric) */
    TBAG_CONSTEXPR static char const * const ATT_ORIENTATION = "orientation";

    /** Width of a grid cell */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** Height of a grid cell */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";
};

struct Image
{
    /** Used for embedded images, in combination with a data child element. Valid values are file extensions like png, gif, jpg, bmp, etc. */
    TBAG_CONSTEXPR static char const * const ATT_FORMAT = "format";

    /** Used by some versions of Tiled Java. Deprecated and unsupported by Tiled Qt. */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The reference to the tileset image file (Tiled supports most common image formats). */
    TBAG_CONSTEXPR static char const * const ATT_SOURCE = "source";

    /** Defines a specific color that is treated as transparent (example value: “#FF00FF” for magenta). Up until Tiled 0.12, this value is written out without a # but this is planned to change. */
    TBAG_CONSTEXPR static char const * const ATT_TRANS = "trans";

    /** The image width in pixels (optional, used for tile index correction when the image changes) */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The image height in pixels (optional) */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";
};

struct TerrainTypes
{
};

struct Terrain
{
    /** The name of the terrain type. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The local tile-id of the tile that represents the terrain visually. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";
};

struct Tile
{
    /** The local tile ID within its tileset. */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The type of the tile. Refers to an object type and is used by tile objects. (optional) (since 1.0) */
    TBAG_CONSTEXPR static char const * const ATT_TYPE = "type";

    /** Defines the terrain type of each corner of the tile, given as comma-separated indexes in the terrain types array in the order top-left, top-right, bottom-left, bottom-right. Leaving out a value means that corner has no terrain. (optional) */
    TBAG_CONSTEXPR static char const * const ATT_TERRAIN = "terrain";

    /** A percentage indicating the probability that this tile is chosen when it competes with others while editing with the terrain tool. (optional) */
    TBAG_CONSTEXPR static char const * const ATT_PROBABILITY = "probability";
};

struct Animation
{
};

struct Frame
{
    /** The local ID of a tile within the parent <tileset>. */
    TBAG_CONSTEXPR static char const * const ATT_TILEID = "tileid";

    /** How long (in milliseconds) this frame should be displayed before advancing to the next frame. */
    TBAG_CONSTEXPR static char const * const ATT_DURATION = "duration";
};

struct WangSets
{
};

struct WangSet
{
    /** The name of the Wang set. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The tile ID of the tile representing this Wang set. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";
};

struct WangCornerColor
{
    /** The name of this color. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The color in #RRGGBB format (example: #c17d11). */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /** The tile ID of the tile representing this color. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";

    /** The relative probability that this color is chosen over others in case of multiple options. */
    TBAG_CONSTEXPR static char const * const ATT_PROBABILITY = "probability";
};

struct WangEdgeColor
{
    /** The name of this color. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The color in #RRGGBB format (example: #c17d11). */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /** The tile ID of the tile representing this color. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";

    /** The relative probability that this color is chosen over others in case of multiple options. */
    TBAG_CONSTEXPR static char const * const ATT_PROBABILITY = "probability";
};

struct WangTile
{
    /** The tile ID. */
    TBAG_CONSTEXPR static char const * const ATT_TILEID = "tileid";

    /** The Wang ID, which is a 32-bit unsigned integer stored in the format 0xCECECECE (where each C is a corner color and each E is an edge color, from right to left clockwise, starting with the top edge) */
    TBAG_CONSTEXPR static char const * const ATT_WANGID = "wangid";
};

struct Layer
{
    /**
     * Unique ID of the layer.
     * Each layer that added to a map gets a unique id.
     * Even if a layer is deleted, no layer ever gets the same ID.
     * Can not be changed in Tiled. (since Tiled 1.2)
     */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the layer. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /**
     * The x coordinate of the layer in tiles.
     * Defaults to 0 and can not be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /**
     * The y coordinate of the layer in tiles.
     * Defaults to 0 and can not be changed in Tiled.
     */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

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

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    /** Rendering offset for this layer in pixels. Defaults to 0. (since 0.14) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /** Rendering offset for this layer in pixels. Defaults to 0. (since 0.14) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";
};

using Layers = std::vector<Layer>;

struct Data
{
    /** The encoding used to encode the tile layer data. When used, it can be “base64” and “csv” at the moment. */
    TBAG_CONSTEXPR static char const * const ATT_ENCODING = "encoding";

    /** The compression used to compress the tile layer data. Tiled supports “gzip” and “zlib”. */
    TBAG_CONSTEXPR static char const * const ATT_COMPRESSION = "compression";
};

struct Chunk
{
    /** The x coordinate of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** The y coordinate of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The width of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The height of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";
};

//struct Tile
//{
//    /** The global tile ID (default: 0). */
//    TBAG_CONSTEXPR static char const * const ATT_GID = "gid";
//};

struct ObjectGroup
{
    /** Unique ID of the layer. Each layer that added to a map gets a unique id. Even if a layer is deleted, no layer ever gets the same ID. Can not be changed in Tiled. (since Tiled 1.2) */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the object group. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The color used to display the objects in this group. */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /** The x coordinate of the object group in tiles. Defaults to 0 and can no longer be changed in Tiled. */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** The y coordinate of the object group in tiles. Defaults to 0 and can no longer be changed in Tiled. */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The width of the object group in tiles. Meaningless. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The height of the object group in tiles. Meaningless. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";

    /** Rendering offset for this object group in pixels. Defaults to 0. (since 0.14) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /** Rendering offset for this object group in pixels. Defaults to 0. (since 0.14) */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";

    /** Whether the objects are drawn according to the order of appearance (“index”) or sorted by their y-coordinate (“topdown”). Defaults to “topdown”. */
    TBAG_CONSTEXPR static char const * const ATT_DRAWORDER = "draworder";
};

struct Object
{
    /** Unique ID of the object. Each object that is placed on a map gets a unique id. Even if an object was deleted, no object gets the same ID. Can not be changed in Tiled. (since Tiled 0.11) */
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
};

struct Ellipse
{
};

struct Point
{
};

struct Polygon
{
    /** A list of x,y coordinates in pixels. */
    TBAG_CONSTEXPR static char const * const ATT_POINTS = "points";
};

struct Polyline
{
    /** A list of x,y coordinates in pixels. */
    TBAG_CONSTEXPR static char const * const ATT_POINTS = "points";
};

struct Text
{
    /** The font family used (default: “sans-serif”) */
    TBAG_CONSTEXPR static char const * const ATT_FONTFAMILY = "fontfamily";

    /** The size of the font in pixels (not using points, because other sizes in the TMX format are also using pixels) (default: 16) */
    TBAG_CONSTEXPR static char const * const ATT_PIXELSIZE = "pixelsize";

    /** Whether word wrapping is enabled (1) or disabled (0). Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_WRAP = "wrap";

    /** Color of the text in #AARRGGBB or #RRGGBB format (default: #000000) */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /** Whether the font is bold (1) or not (0). Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_BOLD = "bold";

    /** Whether the font is italic (1) or not (0). Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_ITALIC = "italic";

    /** Whether a line should be drawn below the text (1) or not (0). Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_UNDERLINE = "underline";

    /** Whether a line should be drawn through the text (1) or not (0). Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_STRIKEOUT = "strikeout";

    /** Whether kerning should be used while rendering the text (1) or not (0). Default to 1. */
    TBAG_CONSTEXPR static char const * const ATT_KERNING = "kerning";

    /** Horizontal alignment of the text within the object (left (default), center, right or justify (since Tiled 1.2.1)) */
    TBAG_CONSTEXPR static char const * const ATT_HALIGN = "halign";

    /** Vertical alignment of the text within the object (top (default), center or bottom) */
    TBAG_CONSTEXPR static char const * const ATT_VALIGN = "valign";
};

struct ImageLayer
{
    /** Unique ID of the layer. Each layer that added to a map gets a unique id. Even if a layer is deleted, no layer ever gets the same ID. Can not be changed in Tiled. (since Tiled 1.2) */
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
};

struct Group
{
    /** Unique ID of the layer. Each layer that added to a map gets a unique id. Even if a layer is deleted, no layer ever gets the same ID. Can not be changed in Tiled. (since Tiled 1.2) */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /** The name of the group layer. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** Rendering offset of the group layer in pixels. Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETX = "offsetx";

    /** Rendering offset of the group layer in pixels. Defaults to 0. */
    TBAG_CONSTEXPR static char const * const ATT_OFFSETY = "offsety";

    /** The opacity of the layer as a value from 0 to 1. Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_OPACITY = "opacity";

    /** Whether the layer is shown (1) or hidden (0). Defaults to 1. */
    TBAG_CONSTEXPR static char const * const ATT_VISIBLE = "visible";
};

/**
 * Example:@n
 * @code{.xml}
 *  <?xml version="1.0" encoding="UTF-8"?>
 *  <map version="1.2"
 *       tiledversion="1.2.3"
 *       orientation="orthogonal"
 *       renderorder="right-down"
 *       width="100"
 *       height="100"
 *       tilewidth="32"
 *       tileheight="32"
 *       infinite="0"
 *       nextlayerid="2"
 *       nextobjectid="1">
 *    <!-- ... -->
 *  </map>
 * @endcode
 */
struct Map
{
    /**
     * The TMX format version.
     * Was "1.0" so far, and will be incremented to match minor Tiled releases.
     */
    TBAG_CONSTEXPR static char const * const ATT_VERSION = "version";

    /**
     * The Tiled version used to save the file (since Tiled 1.0.1).
     * May be a date (for snapshot builds).
     */
    TBAG_CONSTEXPR static char const * const ATT_TILEDVERSION = "tiledversion";

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

    /**
     * For staggered and hexagonal maps,
     * determines whether the "even" or "odd" indexes along the staggered axis are shifted.
     * (since 0.11)
     */
    TBAG_CONSTEXPR static char const * const ATT_STAGGERINDEX = "staggerindex";

    /** The background color of the map.
     * (optional, may include alpha value since 0.15 in the form #AARRGGBB)
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
        ORTHOGONAL,
        ISOMETRIC,
        STAGGERED,
        HEXAGONAL,
    };

    enum class RenderOrder
    {
        RIGHT_DOWN,
        RIGHT_UP,
        LEFT_DOWN,
        LEFT_UP,
    };

    enum class StaggerAxis
    {
        X, Y,
    };

    enum class StaggerIndex
    {
        ODD, EVEN,
    };

    std::string version;
    std::string tiled_version;
    Orientation orientation = Orientation::ORTHOGONAL;
    RenderOrder render_order = RenderOrder::RIGHT_DOWN;
    int width = 0;
    int height = 0;
    int tile_width = 0;
    int tile_height = 0;
    int hexside_length = 0;
    StaggerAxis stagger_axis = StaggerAxis::X;
    StaggerIndex stagger_index = StaggerIndex::ODD;
    std::string background_color;
    int next_layer_id = 0;
    int next_object_id = 0;
    bool infinite = false;

    Properties properties;
};

/**
 * Tmx class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 */
class TBAG_API Tmx
{
public:
    Tmx();
    Tmx(Tmx const & obj);
    Tmx(Tmx && obj) TBAG_NOEXCEPT;
    ~Tmx();

public:
    Tmx & operator =(Tmx const & obj);
    Tmx & operator =(Tmx && obj) TBAG_NOEXCEPT;

public:
    void copy(Tmx const & obj);
    void swap(Tmx & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Tmx & lh, Tmx & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__

