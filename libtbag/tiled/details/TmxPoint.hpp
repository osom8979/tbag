/**
 * @file   TmxPoint.hpp
 * @brief  TmxPoint class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINT_HPP__

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
 * 'point' element.
 *
 * @author zer0
 * @date   2019-06-23
 *
 * @remarks
 *  Used to mark an object as a point.
 *  The existing x and y attributes are used to determine the position of the point.
 */
struct TBAG_API TmxPoint : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "point";

    TBAG_CONSTEXPR static char const * const ATT_X = "x";
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    int x;
    int y;

    TmxPoint();
    TmxPoint(int x_, int y_);
    ~TmxPoint();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPOINT_HPP__

