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

    using       iterator = typename Points::iterator;
    using const_iterator = typename Points::const_iterator;
    using       reverse_iterator = typename Points::reverse_iterator;
    using const_reverse_iterator = typename Points::const_reverse_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(points.begin())
    { return points.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(points.end())
    { return points.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(points.begin())
    { return points.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(points.end())
    { return points.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(points.cbegin())
    { return points.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(points.cend())
    { return points.cend(); }

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(points.rbegin())
    { return points.rbegin(); }
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(points.rend())
    { return points.rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(points.rbegin())
    { return points.rbegin(); }
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(points.rend())
    { return points.rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(points.crbegin())
    { return points.crbegin(); }
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(points.crend())
    { return points.crend(); }

    bool empty() const;
    std::size_t size() const;
    void clear();

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

