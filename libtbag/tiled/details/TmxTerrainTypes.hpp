/**
 * @file   TmxTerrainTypes.hpp
 * @brief  TmxTerrainTypes class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxTerrain.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxTerrainTypes class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxTerrainTypes : protected libtbag::dom::xml::XmlHelper
{
    using Terrains = std::vector<TmxTerrain>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "terraintypes";

    Terrains terrains;

    TmxTerrainTypes();
    TmxTerrainTypes(Terrains const & t);
    ~TmxTerrainTypes();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTERRAINTYPES_HPP__

