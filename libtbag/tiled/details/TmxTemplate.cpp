/**
 * @file   TmxTemplate.cpp
 * @brief  TmxTemplate class implementation.
 * @author zer0
 * @date   2019-08-15
 */

#include <libtbag/tiled/details/TmxTemplate.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTemplate::TmxTemplate()
{
    // EMPTY.
}

TmxTemplate::~TmxTemplate()
{
    // EMPTY.
}

Err TmxTemplate::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    foreachElement(elem, TmxTileSet::TAG_NAME, [&](Element const & e){
        TmxTileSet tile_set;
        if (isSuccess(tile_set.read(e))) {
            tile_sets.push_back(std::move(tile_set));
        }
    });

    foreachElement(elem, TmxObject::TAG_NAME, [&](Element const & e){
        TmxObject object;
        if (isSuccess(object.read(e))) {
            objects.push_back(std::move(object));
        }
    });

    return E_SUCCESS;
}

Err TmxTemplate::read(std::string const & xml)
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

Err TmxTemplate::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    for (auto & tile_set : tile_sets) {
        newElement(elem, TmxTileSet::TAG_NAME, [&](Element & p){
            tile_set.write(p);
        });
    }

    for (auto & object : objects) {
        newElement(elem, TmxObject::TAG_NAME, [&](Element & p){
            object.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxTemplate::write(std::string & xml) const
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

