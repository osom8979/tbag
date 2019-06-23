/**
 * @file   TmxPolygon.hpp
 * @brief  TmxPolygon class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYGON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYGON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tiled/details/TmxPointsCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxPolygon class prototype.
 *
 * @author zer0
 * @date   2019-06-23
 *
 * @remarks
 *  Each polygon object is made up of a space-delimited list of x,y coordinates.
 *  The origin for these coordinates is the location of the parent object.
 *  By default, the first point is created as 0,0 denoting that the point
 *  will originate exactly where the object is placed.
 */
struct TBAG_API TmxPolygon : protected libtbag::tiled::details::TmxPointsCommon
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "polygon";

    Points points;

    TmxPolygon();
    TmxPolygon(Points const & p);
    ~TmxPolygon();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYGON_HPP__

