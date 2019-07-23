/**
 * @file   TmxTerrain.cpp
 * @brief  TmxTerrain class implementation.
 * @author zer0
 * @date   2019-07-22
 */

#include <libtbag/tiled/details/TmxTerrain.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTerrain::TmxTerrain()
{
    // EMPTY.
}

TmxTerrain::TmxTerrain(std::string const & n, int t, TmxProperties const & p)
        : name(n), tile(t), properties(p)
{
    // EMPTY.
}

TmxTerrain::~TmxTerrain()
{
    // EMPTY.
}

Err TmxTerrain::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_TILE, tile);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    return E_SUCCESS;
}

Err TmxTerrain::read(std::string const & xml)
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

Err TmxTerrain::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_TILE, tile);

    newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
        properties.write(d);
    });

    return E_SUCCESS;
}

Err TmxTerrain::write(std::string & xml) const
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

