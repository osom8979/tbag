/**
 * @file   TmxWangSet.hpp
 * @brief  TmxWangSet class prototype.
 * @author zer0
 * @date   2019-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxWangCornerColor.hpp>
#include <libtbag/tiled/details/TmxWangEdgeColor.hpp>
#include <libtbag/tiled/details/TmxWangTile.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxWangSet class prototype.
 *
 * @author zer0
 * @date   2019-07-14
 *
 * @remarks
 *  Defines a list of corner colors and a list of edge colors,
 *  and any number of Wang tiles using these colors.
 */
struct TBAG_API TmxWangSet : protected libtbag::dom::xml::XmlHelper
{
    using WangCornerColors = std::vector<TmxWangCornerColor>;
    using WangEdgeColors = std::vector<TmxWangEdgeColor>;
    using WangTiles = std::vector<TmxWangTile>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "wangset";

    /** The name of the Wang set. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The tile ID of the tile representing this Wang set. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";

    std::string name;
    int tile;

    WangCornerColors wang_corner_colors;
    WangEdgeColors wang_edge_colors;
    WangTiles wang_tiles;

    TmxWangSet();
    TmxWangSet(std::string const & n, int t);
    virtual ~TmxWangSet();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSET_HPP__

