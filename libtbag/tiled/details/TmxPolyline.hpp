/**
 * @file   TmxPolyline.hpp
 * @brief  TmxPolyline class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYLINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYLINE_HPP__

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
 * TmxPolyline class prototype.
 *
 * @author zer0
 * @date   2019-06-23
 *
 * @remarks
 *  A polyline follows the same placement definition as a polygon object.
 */
struct TBAG_API TmxPolyline : protected libtbag::tiled::details::TmxPointsCommon
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "polyline";

    Points points;

    TmxPolyline();
    TmxPolyline(Points const & p);
    ~TmxPolyline();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOLYLINE_HPP__

