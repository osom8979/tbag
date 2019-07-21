/**
 * @file   TmxWangCornerColor.cpp
 * @brief  TmxWangCornerColor class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxWangCornerColor.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxWangCornerColor::TmxWangCornerColor()
{
    // EMPTY.
}

TmxWangCornerColor::TmxWangCornerColor(std::string const & n, Color const & c, int t, int p)
        : name(n), color(c), tile(t), probability(p)
{
    // EMPTY.
}

TmxWangCornerColor::~TmxWangCornerColor()
{
    // EMPTY.
}

Err TmxWangCornerColor::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_NAME, name);

    std::string color_text;
    optAttr(elem, ATT_COLOR, color_text);
    color.fromRgb24String(color_text);

    optAttr(elem, ATT_TILE, tile);
    optAttr(elem, ATT_PROBABILITY, probability);

    return E_SUCCESS;
}

Err TmxWangCornerColor::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxWangCornerColor::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_COLOR, color.toRgb24String());
    setAttr(elem, ATT_TILE, tile);
    setAttr(elem, ATT_PROBABILITY, probability);
    return E_SUCCESS;
}

Err TmxWangCornerColor::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
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

