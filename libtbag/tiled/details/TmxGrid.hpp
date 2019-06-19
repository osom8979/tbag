/**
 * @file   TmxGrid.hpp
 * @brief  TmxGrid class prototype.
 * @author zer0
 * @date   2019-06-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGRID_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGRID_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * 'grid' element.
 *
 * @author zer0
 * @date   2019-06-19
 *
 * @remarks
 *  This element is only used in case of isometric orientation,
 *  and determines how tile overlays for terrain and collision information are rendered.
 */
struct TBAG_API TmxGrid : public libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "grid";

    /** Width of a grid cell. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** Height of a grid cell. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    /** Orientation of the grid for the tiles in this tileset (orthogonal or isometric) */
    TBAG_CONSTEXPR static char const * const ATT_ORIENTATION = "orientation";
    TBAG_CONSTEXPR static char const * const VAL_ORTHOGONAL = "orthogonal";
    TBAG_CONSTEXPR static char const * const VAL_ISOMETRIC = "isometric";

    int width;
    int height;

    enum class Orientation
    {
        NONE, ORTHOGONAL, ISOMETRIC
    };

    Orientation orientation;

    TmxGrid();
    TmxGrid(int w, int h, Orientation o = Orientation::NONE);
    ~TmxGrid();

    static Orientation getOrientation(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getOrientationName(Orientation o) TBAG_NOEXCEPT;

    Err read(Element const & elem);
    Err dump(Element & elem) const;
    Err dumpToParent(Element & elem) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXGRID_HPP__

