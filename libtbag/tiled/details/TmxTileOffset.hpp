/**
 * @file   TmxTileOffset.hpp
 * @brief  TmxTileOffset class prototype.
 * @author zer0
 * @date   2019-06-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILEOFFSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILEOFFSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * 'tileoffset' element.
 *
 * @author zer0
 * @date   2019-06-19
 *
 * @remarks
 *  This element is used to specify an offset in pixels,
 *  to be applied when drawing a tile from the related tileset.
 *  When not present, no offset is applied.
 */
struct TBAG_API TmxTileOffset : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "tileoffset";

    TBAG_CONSTEXPR static char const * const ATT_X = "x";
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    int x; ///< Horizontal offset in pixels
    int y; ///< Vertical offset in pixels (positive is down)

    TmxTileOffset();
    TmxTileOffset(int x_, int y_);
    ~TmxTileOffset();

    Err read(Element const & elem);
    Err dump(Element & elem) const;
    Err dumpToParent(Element & elem) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTILEOFFSET_HPP__

