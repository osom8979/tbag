/**
 * @file   TmxImageLayer.cpp
 * @brief  TmxImageLayer class implementation.
 * @author zer0
 * @date   2019-08-11
 */

#include <libtbag/tiled/details/TmxImageLayer.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxImageLayer::TmxImageLayer()
{
    // EMPTY.
}

TmxImageLayer::~TmxImageLayer()
{
    // EMPTY.
}

Err TmxImageLayer::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_OFFSETX, offsetx);
    optAttr(elem, ATT_OFFSETY, offsety);
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_OPACITY, opacity);
    optAttr(elem, ATT_VISIBLE, visible);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    auto const * image_elem = elem.FirstChildElement(TmxImage::TAG_NAME);
    if (image_elem != nullptr) {
        image.read(*image_elem);
    }

    return E_SUCCESS;
}

Err TmxImageLayer::read(std::string const & xml)
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

Err TmxImageLayer::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_OFFSETX, offsetx);
    setAttr(elem, ATT_OFFSETY, offsety);
    setAttr(elem, ATT_OPACITY, opacity);
    setAttr(elem, ATT_VISIBLE, visible);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }

    newElement(elem, TmxImage::TAG_NAME, [&](Element & d){
        image.write(d);
    });

    return E_SUCCESS;
}

Err TmxImageLayer::write(std::string & xml) const
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

