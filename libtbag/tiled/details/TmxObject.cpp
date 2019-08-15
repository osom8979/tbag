/**
 * @file   TmxObject.cpp
 * @brief  TmxObject class implementation.
 * @author zer0
 * @date   2019-07-24
 */

#include <libtbag/tiled/details/TmxObject.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxObject::TmxObject()
{
    // EMPTY.
}

TmxObject::TmxObject(int x_, int y_) : x(x_), y(y_)
{
    // EMPTY.
}

TmxObject::~TmxObject()
{
    // EMPTY.
}

Err TmxObject::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_TYPE, type);
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);
    optAttr(elem, ATT_ROTATION, rotation);
    optAttr(elem, ATT_GID, gid);
    optAttr(elem, ATT_VISIBLE, visible);
    optAttr(elem, ATT_TEMPLATE, template_file);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }
    auto const * ellipse_elem = elem.FirstChildElement(TmxEllipse::TAG_NAME);
    if (ellipse_elem != nullptr) {
        ellipse.read(*ellipse_elem);
    }
    auto const * polygon_elem = elem.FirstChildElement(TmxPolygon::TAG_NAME);
    if (polygon_elem != nullptr) {
        polygon.read(*polygon_elem);
    }
    auto const * polyline_elem = elem.FirstChildElement(TmxPolyline::TAG_NAME);
    if (polyline_elem != nullptr) {
        polyline.read(*polyline_elem);
    }
    auto const * text_elem = elem.FirstChildElement(TmxText::TAG_NAME);
    if (text_elem != nullptr) {
        text.read(*text_elem);
    }
    auto const * image_elem = elem.FirstChildElement(TmxImage::TAG_NAME);
    if (image_elem != nullptr) {
        image.read(*image_elem);
    }

    return E_SUCCESS;
}

Err TmxObject::read(std::string const & xml)
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

Err TmxObject::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_TYPE, type);
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    setAttr(elem, ATT_ROTATION, rotation);
    setAttr(elem, ATT_GID, gid);
    setAttr(elem, ATT_VISIBLE, visible);
    setAttr(elem, ATT_TEMPLATE, template_file);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }
    if (!ellipse.empty()) {
        newElement(elem, TmxEllipse::TAG_NAME, [&](Element & d){
            ellipse.write(d);
        });
    }
    if (!polygon.empty()) {
        newElement(elem, TmxPolygon::TAG_NAME, [&](Element & d){
            polygon.write(d);
        });
    }
    if (!polyline.empty()) {
        newElement(elem, TmxPolyline::TAG_NAME, [&](Element & d){
            polyline.write(d);
        });
    }
    if (!text.data.empty()) {
        newElement(elem, TmxText::TAG_NAME, [&](Element & d){
            text.write(d);
        });
    }
    if (!image.data.empty()) {
        newElement(elem, TmxImage::TAG_NAME, [&](Element & d){
            image.write(d);
        });
    }

    return E_SUCCESS;
}

Err TmxObject::write(std::string & xml) const
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

