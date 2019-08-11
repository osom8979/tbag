/**
 * @file   TmxTile.hpp
 * @brief  TmxTile class prototype.
 * @author zer0
 * @date   2019-08-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/tiled/details/TmxImage.hpp>
#include <libtbag/tiled/details/TmxObjectGroup.hpp>
#include <libtbag/tiled/details/TmxAnimation.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTile class prototype.
 *
 * @author zer0
 * @date   2019-08-11
 */
struct TBAG_API TmxTile : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "tile";

    /** The local tile ID within its tileset. */
    TBAG_CONSTEXPR static char const * const ATT_ID = "id";

    /**
     * The type of the tile. Refers to an object type and is used by tile objects.
     * (optional) (since 1.0)
     */
    TBAG_CONSTEXPR static char const * const ATT_TYPE = "type";

    /**
     * Defines the terrain type of each corner of the tile,
     * given as comma-separated indexes in the terrain types array in the order
     * top-left, top-right, bottom-left, bottom-right.
     * Leaving out a value means that corner has no terrain.
     * (optional)
     */
    TBAG_CONSTEXPR static char const * const ATT_TERRAIN = "terrain";

    /**
     * A percentage indicating the probability that this tile is chosen
     * when it competes with others while editing with the terrain tool.
     * (optional)
     */
    TBAG_CONSTEXPR static char const * const ATT_PROBABILITY = "probability";

    int id = 0;
    std::string type;

    struct Terrain
    {
        bool enable = false;
        int top_left = 0;
        int top_right = 0;
        int bottom_left = 0;
        int bottom_right = 0;
    };

    Terrain terrain;

    float probability = 0.0f;

    TmxProperties properties;
    TmxImage image;
    TmxObjectGroup object_group;
    TmxAnimation animation;

    TmxTile();
    ~TmxTile();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILE_HPP__

