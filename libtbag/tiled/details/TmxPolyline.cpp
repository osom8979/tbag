/**
 * @file   TmxPolyline.cpp
 * @brief  TmxPolyline class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxPolyline.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxPolyline::TmxPolyline() : points()
{
    // EMPTY.
}

TmxPolyline::TmxPolyline(Points const & p) : points(p)
{
    // EMPTY.
}

TmxPolyline::~TmxPolyline()
{
    // EMPTY.
}

bool TmxPolyline::empty() const
{
    return points.empty();
}

std::size_t TmxPolyline::size() const
{
    return points.size();
}

Err TmxPolyline::read(Element const & elem)
{
    return readToPoints(TAG_NAME, elem, points);
}

Err TmxPolyline::read(std::string const & xml)
{
    return readToPoints(TAG_NAME, xml, points);
}

Err TmxPolyline::write(Element & elem) const
{
    return writeFromPoints(TAG_NAME, points, elem);
}

Err TmxPolyline::write(std::string & xml) const
{
    return writeFromPoints(TAG_NAME, points, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

