/**
 * @file   TmxPointsCommon.cpp
 * @brief  TmxPointsCommon class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxPointsCommon.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

Err TmxPointsCommon::convertStringToPoints(std::string const & text, Points & points)
{
    using namespace libtbag::string;
    int x, y;
    for (auto & point : splitTokens(text, POINTS_DELIMITER)) {
        auto const xy = splitTokens(point, XY_DELIMITER);
        if (xy.size() != 2) {
            return E_PARSING;
        }
        if (!toVal(xy[0], x)) {
            return E_PARSING;
        }
        if (!toVal(xy[1], y)) {
            return E_PARSING;
        }
        points.emplace_back(x, y);
    }
    return E_SUCCESS;
}

Err TmxPointsCommon::convertPointsToString(Points const & points, std::string & text)
{
    std::stringstream ss;
    auto itr = points.begin();
    auto const END = points.end();
    if (itr != END) {
        ss << itr->x << XY_DELIMITER << itr->y;
        ++itr;
    }
    for (; itr != END; ++itr) {
        ss << POINTS_DELIMITER << itr->x << XY_DELIMITER << itr->y;
    }
    text = ss.str();
    return E_SUCCESS;
}

Err TmxPointsCommon::readToPoints(std::string const & tag, Element const & elem, Points & points)
{
    if (tag != elem.Name()) {
        return E_ILLARGS;
    }

    std::string points_text;
    optAttr(elem, ATT_POINTS, points_text);

    Points points_temp;
    auto const CODE = convertStringToPoints(points_text, points_temp);
    if (isSuccess(CODE)) {
        points.swap(points_temp);
    }
    return CODE;
}

Err TmxPointsCommon::readToPoints(std::string const & tag, std::string const & xml, Points & points)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(tag.c_str());
    assert(elem != nullptr);
    return readToPoints(tag, *elem, points);
}

Err TmxPointsCommon::writeFromPoints(std::string const & tag, Points const & points, Element & elem)
{
    if (tag != elem.Name()) {
        return E_ILLARGS;
    }
    std::string points_text;
    auto const CODE = convertPointsToString(points, points_text);
    if (isFailure(CODE)) {
        return CODE;
    }
    setAttr(elem, ATT_POINTS, points_text);
    return E_SUCCESS;
}

Err TmxPointsCommon::writeFromPoints(std::string const & tag, Points const & points, std::string & xml)
{
    Document doc;
    auto * new_elem = newElement(doc, tag.c_str());
    assert(new_elem != nullptr);
    auto const CODE = writeFromPoints(tag, points, *new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

