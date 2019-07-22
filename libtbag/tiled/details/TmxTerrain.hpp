/**
 * @file   TmxTerrain.hpp
 * @brief  TmxTerrain class prototype.
 * @author zer0
 * @date   2019-07-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperty.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTerrain class prototype.
 *
 * @author zer0
 * @date   2019-07-22
 */
struct TBAG_API TmxTerrain : protected libtbag::dom::xml::XmlHelper
{
    using Properties = std::vector<TmxProperty>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "terrain";

    /** The name of the terrain type. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The local tile-id of the tile that represents the terrain visually. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";

    std::string name;
    int tile;

    Properties properties;

    TmxTerrain();
    TmxTerrain(std::string const & n, int t, Properties const & p);
    ~TmxTerrain();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAIN_HPP__

