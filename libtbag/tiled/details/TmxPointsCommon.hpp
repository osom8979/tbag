/**
 * @file   TmxPointsCommon.hpp
 * @brief  TmxPointsCommon class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINTSCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINTSCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/geometry/Point2.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxPointsCommon class prototype.
 *
 * @author zer0
 * @date   2019-06-23
 */
struct TBAG_API TmxPointsCommon : protected libtbag::dom::xml::XmlHelper
{
    using Point = libtbag::geometry::Point2i;
    using Points = std::vector<Point>;

    /** A list of x,y coordinates in pixels. */
    TBAG_CONSTEXPR static char const * const ATT_POINTS = "points";
    TBAG_CONSTEXPR static char const * const POINTS_DELIMITER = " ";
    TBAG_CONSTEXPR static char const * const XY_DELIMITER = ",";

    static Err convertStringToPoints(std::string const & text, Points & points);
    static Err convertPointsToString(Points const & points, std::string & text);

    static Err readToPoints(std::string const & tag, Element const & elem, Points & points);
    static Err readToPoints(std::string const & tag, std::string const & xml, Points & points);

    static Err writeFromPoints(std::string const & tag, Points const & points, Element & elem);
    static Err writeFromPoints(std::string const & tag, Points const & points, std::string & xml);
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINTSCOMMON_HPP__

