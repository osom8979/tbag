/**
 * @file   TmxWangCornerColor.hpp
 * @brief  TmxWangCornerColor class prototype.
 * @author zer0
 * @date   2019-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGCORNERCOLOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGCORNERCOLOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/graphic/Color.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxWangCornerColor class prototype.
 *
 * @author zer0
 * @date   2019-07-14
 *
 * @remarks
 *  A color that can be used to define the corner of a Wang tile.
 */
struct TBAG_API TmxWangCornerColor : protected libtbag::dom::xml::XmlHelper
{
    using Color = libtbag::graphic::Color;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "wangcornercolor";

    /** The name of this color. */
    TBAG_CONSTEXPR static char const * const ATT_NAME = "name";

    /** The color in <code>#RRGGBB</code> format. */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";

    /** The tile ID of the tile representing this color. */
    TBAG_CONSTEXPR static char const * const ATT_TILE = "tile";

    /** The relative probability that this color is chosen over others in case of multiple options. */
    TBAG_CONSTEXPR static char const * const ATT_PROBABILITY = "probability";

    std::string name;
    Color color;
    int tile;
    float probability;

    TmxWangCornerColor();
    TmxWangCornerColor(std::string const & n, Color const & c, int t, int p);
    ~TmxWangCornerColor();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGCORNERCOLOR_HPP__

