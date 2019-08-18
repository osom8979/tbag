/**
 * @file   TmxTileSet.hpp
 * @brief  TmxTileSet class prototype.
 * @author zer0
 * @date   2019-08-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILESET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILESET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxTileOffset.hpp>
#include <libtbag/tiled/details/TmxGrid.hpp>
#include <libtbag/tiled/details/TmxImage.hpp>
#include <libtbag/tiled/details/TmxTerrainTypes.hpp>
#include <libtbag/tiled/details/TmxTile.hpp>
#include <libtbag/tiled/details/TmxWangSets.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTileSet class prototype.
 *
 * @author zer0
 * @date   2019-08-15
 *
 * @remarks
 *  If there are multiple <code>tileset</code> elements,
 *  they are in ascending order of their <code>firstgid</code> attribute.
 *  The first tileset always has a <code>firstgid</code> value of 1. Since Tiled 0.15,
 *  image collection tilesets do not necessarily number their tiles
 *  consecutively since gaps can occur when removing tiles.
 */
struct TBAG_API TmxTileSet : protected libtbag::dom::xml::XmlHelper
{
    using TileOffsets = std::vector<TmxTileOffset>;
    using Grids = std::vector<TmxGrid>;
    using Images = std::vector<TmxImage>;
    using Tiles = std::vector<TmxTile>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "tileset";

    /**
     * The first global tile ID of this tileset
     * (this global ID maps to the first tile in this tileset).
     */
    TBAG_CONSTEXPR static char const * const ATT_FIRSTGID = "firstgid";

    /**
     * If this tileset is stored in an external TSX (Tile Set XML) file,
     * this attribute refers to that file.
     * That TSX file has the same structure as the <code>tileset</code> element described here.
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

    /** The margin around the tiles in this tileset (applies to the tileset image). */
    TBAG_CONSTEXPR static char const * const ATT_MARGIN = "margin";

    /** The number of tiles in this tileset (since 0.13) */
    TBAG_CONSTEXPR static char const * const ATT_TILECOUNT = "tilecount";

    /**
     * The number of tile columns in the tileset.
     * For image collection tilesets it is editable and is used when displaying the tileset.
     * (since 0.15) */
    TBAG_CONSTEXPR static char const * const ATT_COLUMNS = "columns";

    int firstgid;
    std::string source;
    std::string name;
    int tilewidth;
    int tileheight;
    int spacing;
    int margin;
    int tilecount;
    int columns;

    TmxProperties properties;
    TmxTerrainTypes terrain_types;
    TmxWangSets wang_sets; // (since 1.1)

    TileOffsets tile_offsets;
    Grids grids; // (since 1.0)
    Images images;
    Tiles tiles;

    TmxTileSet();
    ~TmxTileSet();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILESET_HPP__

