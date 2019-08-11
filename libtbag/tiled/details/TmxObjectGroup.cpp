/**
 * @file   TmxObjectGroup.cpp
 * @brief  TmxObjectGroup class implementation.
 * @author zer0
 * @date   2019-08-11
 */

#include <libtbag/tiled/details/TmxObjectGroup.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxObjectGroup::TmxObjectGroup()
{
    // EMPTY.
}

TmxObjectGroup::~TmxObjectGroup()
{
    // EMPTY.
}

TmxObjectGroup::DrawOrder TmxObjectGroup::getDrawOrder(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_INDEX) {
        return DrawOrder::INDEX;
    } else if (text == VAL_TOPDOWN) {
        return DrawOrder::TOPDOWN;
    } else {
        return DrawOrder::NONE;
    }
}

char const * const TmxObjectGroup::getDrawOrderName(DrawOrder order) TBAG_NOEXCEPT
{
    // clang-format off
    switch (order) {
    case DrawOrder::INDEX:   return VAL_INDEX;
    case DrawOrder::TOPDOWN: return VAL_TOPDOWN;
    case DrawOrder::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

Err TmxObjectGroup::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_NAME, name);

    std::string color_text;
    optAttr(elem, ATT_COLOR, color_text);
    color.fromRgb24String(color_text);

    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);
    optAttr(elem, ATT_OPACITY, opacity);
    optAttr(elem, ATT_VISIBLE, visible);
    optAttr(elem, ATT_OFFSETX, offsetx);
    optAttr(elem, ATT_OFFSETY, offsety);

    std::string draw_order_text;
    optAttr(elem, ATT_DRAWORDER, draw_order_text, VAL_TOPDOWN);
    draworder = getDrawOrder(draw_order_text);
    if (draworder == DrawOrder::NONE) {
        return E_PARSING;
    }

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    foreachElement(elem, TmxObject::TAG_NAME, [&](Element const & e){
        TmxObject object;
        if (isSuccess(object.read(e))) {
            objects.push_back(std::move(object));
        }
    });

    return E_SUCCESS;
}

Err TmxObjectGroup::read(std::string const & xml)
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

Err TmxObjectGroup::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_COLOR, color.toRgb24String());
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    setAttr(elem, ATT_OPACITY, opacity);
    setAttr(elem, ATT_VISIBLE, visible);
    setAttr(elem, ATT_OFFSETX, offsetx);
    setAttr(elem, ATT_OFFSETY, offsety);
    setAttr(elem, ATT_DRAWORDER, getDrawOrderName(draworder));

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }

    for (auto & object : objects) {
        newElement(elem, TmxObject::TAG_NAME, [&](Element & p){
            object.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxObjectGroup::write(std::string & xml) const
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

