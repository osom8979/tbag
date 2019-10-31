/**
 * @file   TmxTile.cpp
 * @brief  TmxTile class implementation.
 * @author zer0
 * @date   2019-08-11
 */

#include <libtbag/tiled/details/TmxTile.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTile::TmxTile()
{
    // EMPTY.
}

TmxTile::~TmxTile()
{
    // EMPTY.
}

Err TmxTile::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    auto const code1 = optAttr(elem, ATT_ID, id);
    if (isFailure(code1)) {
        return code1;
    }

    optAttr(elem, ATT_TYPE, type);

    std::string terrain_text;
    auto const code2 = optAttr(elem, ATT_TERRAIN, terrain_text);
    if (isSuccess(code2)) {
        auto const tokens = libtbag::string::splitTokens(terrain_text, ",", false);
        if (tokens.size() != 4) {
            return E_PARSING;
        }
        terrain.enable = true;
        terrain.top_left = libtbag::string::toValue<int>(tokens[0]);
        terrain.top_right = libtbag::string::toValue<int>(tokens[1]);
        terrain.bottom_left = libtbag::string::toValue<int>(tokens[2]);
        terrain.bottom_right = libtbag::string::toValue<int>(tokens[3]);
    }

    optAttr(elem, ATT_PROBABILITY, probability);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }
    auto const * image_elem = elem.FirstChildElement(TmxImage::TAG_NAME);
    if (image_elem != nullptr) {
        image.read(*image_elem);
    }
    auto const * object_group_elem = elem.FirstChildElement(TmxObjectGroup::TAG_NAME);
    if (object_group_elem != nullptr) {
        object_group.read(*object_group_elem);
    }
    auto const * animation_elem = elem.FirstChildElement(TmxAnimation::TAG_NAME);
    if (animation_elem != nullptr) {
        animation.read(*animation_elem);
    }

    return E_SUCCESS;
}

Err TmxTile::read(std::string const & xml)
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

Err TmxTile::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_ID, id);
    if (!type.empty()) {
        setAttr(elem, ATT_TYPE, type);
    }

    if (terrain.enable) {
        using namespace libtbag::string;
        auto const terrain_text = fformat("{},{},{},{}", terrain.top_left, terrain.top_right,
                                          terrain.bottom_left, terrain.bottom_right);
        setAttr(elem, ATT_TERRAIN, terrain_text);
    }
    setAttr(elem, ATT_PROBABILITY, probability);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }
    if (true) {
        newElement(elem, TmxEllipse::TAG_NAME, [&](Element & d){
            image.write(d);
        });
    }
    if (true) {
        newElement(elem, TmxPolygon::TAG_NAME, [&](Element & d){
            object_group.write(d);
        });
    }
    if (!animation.empty()) {
        newElement(elem, TmxPolyline::TAG_NAME, [&](Element & d){
            animation.write(d);
        });
    }

    return E_SUCCESS;
}

Err TmxTile::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeDocumentToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

