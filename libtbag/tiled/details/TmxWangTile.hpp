/**
 * @file   TmxWangTile.hpp
 * @brief  TmxWangTile class prototype.
 * @author zer0
 * @date   2019-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGTILE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGTILE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxWangTile class prototype.
 *
 * @author zer0
 * @date   2019-07-14
 *
 * @remarks
 *  Defines a Wang tile, by referring to a tile in the tileset and associating it with a certain Wang ID.
 */
struct TBAG_API TmxWangTile : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "wangtile";

    /** The tile ID. */
    TBAG_CONSTEXPR static char const * const ATT_TILEID = "tileid";

    /**
     * The Wang ID, which is a 32-bit unsigned integer stored in the format <code>0xCECECECE</code>
     *
     * where each C is a corner color and each E is an edge color,
     * from right to left clockwise, starting with the top edge
     */
    TBAG_CONSTEXPR static char const * const ATT_WANGID = "wangid";

    int tileid;
    std::uint32_t wangid;

    TmxWangTile();
    TmxWangTile(int t, int w);
    virtual ~TmxWangTile();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGTILE_HPP__

