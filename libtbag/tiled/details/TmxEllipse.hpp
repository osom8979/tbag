/**
 * @file   TmxEllipse.hpp
 * @brief  TmxEllipse class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXELLIPSE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXELLIPSE_HPP__

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
 * 'ellipse' element.
 *
 * @author zer0
 * @date   2019-06-23
 *
 * @remarks
 *  Used to mark an object as an ellipse.
 *  The existing x, y, width and height attributes are used to determine the size of the ellipse.
 */
struct TBAG_API TmxEllipse : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "ellipse";

    TBAG_CONSTEXPR static char const * const ATT_X = "x";
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    int x;
    int y;
    int width;
    int height;

    TmxEllipse();
    TmxEllipse(int x_, int y_, int w_, int h_);
    ~TmxEllipse();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXELLIPSE_HPP__

