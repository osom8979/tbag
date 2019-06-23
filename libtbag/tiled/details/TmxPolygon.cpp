/**
 * @file   TmxPolygon.cpp
 * @brief  TmxPolygon class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxPolygon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxPolygon::TmxPolygon() : points()
{
    // EMPTY.
}

TmxPolygon::TmxPolygon(Points const & p) : points(p)
{
    // EMPTY.
}

TmxPolygon::~TmxPolygon()
{
    // EMPTY.
}

Err TmxPolygon::read(Element const & elem)
{
    return readToPoints(TAG_NAME, elem, points);
}

Err TmxPolygon::read(std::string const & xml)
{
    return readToPoints(TAG_NAME, xml, points);
}

Err TmxPolygon::write(Element & elem) const
{
    return writeFromPoints(TAG_NAME, points, elem);
}

Err TmxPolygon::write(std::string & xml) const
{
    return writeFromPoints(TAG_NAME, points, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

