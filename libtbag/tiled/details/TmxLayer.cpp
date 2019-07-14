/**
 * @file   TmxLayer.cpp
 * @brief  TmxLayer class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxLayer.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxLayer::TmxLayer()
{
    // EMPTY.
}

TmxLayer::TmxLayer(int i, std::string const & n, int x_, int y_, int w, int h, int o, int v, int ox, int oy)
        : id(i), name(n), x(x_), y(y_), width(w), height(h), opacity(o), visible(v), offsetx(ox), offsety(oy)
{
    // EMPTY.
}

TmxLayer::~TmxLayer()
{
    // EMPTY.
}

Err TmxLayer::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    auto const code1 = optAttr(elem, ATT_NAME, name);
    if (isFailure(code1)) {
        return code1;
    }
    auto const code2 = optAttr(elem, ATT_WIDTH, width);
    if (isFailure(code2)) {
        return code2;
    }
    auto const code3 = optAttr(elem, ATT_HEIGHT, height);
    if (isFailure(code3)) {
        return code3;
    }

    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_OPACITY, opacity);
    optAttr(elem, ATT_VISIBLE, visible);
    optAttr(elem, ATT_OFFSETX, offsetx);
    optAttr(elem, ATT_OFFSETY, offsety);

    foreachElement(elem, TmxProperty::TAG_NAME, [&](Element const & e){
        TmxProperty property;
        if (isSuccess(property.read(e))) {
            properties.push_back(std::move(property));
        }
    });

    auto const * data_elem = elem.FirstChildElement(TmxData::TAG_NAME);
    if (data_elem != nullptr) {
        data.read(*data_elem);
    }

    return E_SUCCESS;
}

Err TmxLayer::read(std::string const & xml)
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

Err TmxLayer::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    if (name.empty()) {
        return E_ILLSTATE;
    }

    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);

    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_OPACITY, opacity);
    setAttr(elem, ATT_VISIBLE, visible);
    setAttr(elem, ATT_OFFSETX, offsetx);
    setAttr(elem, ATT_OFFSETY, offsety);

    for (auto & property : properties) {
        newElement(elem, TmxProperty::TAG_NAME, [&](Element & p){
            property.write(p);
        });
    }

    newElement(elem, TmxData::TAG_NAME, [&](Element & d){
        data.write(d);
    });

    return E_SUCCESS;
}

Err TmxLayer::write(std::string & xml) const
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

