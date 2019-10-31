/**
 * @file   TmxGroup.cpp
 * @brief  TmxGroup class implementation.
 * @author zer0
 * @date   2019-08-15
 */

#include <libtbag/tiled/details/TmxGroup.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxGroup::TmxGroup()
{
    // EMPTY.
}

TmxGroup::~TmxGroup()
{
    // EMPTY.
}

Err TmxGroup::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_OFFSETX, offsetx);
    optAttr(elem, ATT_OFFSETY, offsety);
    optAttr(elem, ATT_OPACITY, opacity);
    optAttr(elem, ATT_VISIBLE, visible);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    foreachElement(elem, TmxLayer::TAG_NAME, [&](Element const & e){
        TmxLayer layer;
        if (isSuccess(layer.read(e))) {
            layers.push_back(std::move(layer));
        }
    });

    foreachElement(elem, TmxObjectGroup::TAG_NAME, [&](Element const & e){
        TmxObjectGroup object_group;
        if (isSuccess(object_group.read(e))) {
            object_groups.push_back(std::move(object_group));
        }
    });

    foreachElement(elem, TmxImageLayer::TAG_NAME, [&](Element const & e){
        TmxImageLayer image_layer;
        if (isSuccess(image_layer.read(e))) {
            image_layers.push_back(std::move(image_layer));
        }
    });

    foreachElement(elem, TmxGroup::TAG_NAME, [&](Element const & e){
        TmxGroup group;
        if (isSuccess(group.read(e))) {
            groups.push_back(std::move(group));
        }
    });

    return E_SUCCESS;
}

Err TmxGroup::read(std::string const & xml)
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

Err TmxGroup::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_OFFSETX, offsetx);
    setAttr(elem, ATT_OFFSETY, offsety);
    setAttr(elem, ATT_OPACITY, opacity);
    setAttr(elem, ATT_VISIBLE, visible);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }

    for (auto & layer : layers) {
        newElement(elem, TmxLayer::TAG_NAME, [&](Element & p){
            layer.write(p);
        });
    }

    for (auto & object_group : object_groups) {
        newElement(elem, TmxObjectGroup::TAG_NAME, [&](Element & p){
            object_group.write(p);
        });
    }

    for (auto & image_layer : image_layers) {
        newElement(elem, TmxImageLayer::TAG_NAME, [&](Element & p){
            image_layer.write(p);
        });
    }

    for (auto & group : groups) {
        newElement(elem, TmxGroup::TAG_NAME, [&](Element & p){
            group.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxGroup::write(std::string & xml) const
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

