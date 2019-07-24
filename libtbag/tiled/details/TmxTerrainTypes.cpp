/**
 * @file   TmxTerrainTypes.cpp
 * @brief  TmxTerrainTypes class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxTerrainTypes.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTerrainTypes::TmxTerrainTypes()
{
    // EMPTY.
}

TmxTerrainTypes::TmxTerrainTypes(Terrains const & t) : terrains(t)
{
    // EMPTY.
}

TmxTerrainTypes::~TmxTerrainTypes()
{
    // EMPTY.
}

bool TmxTerrainTypes::empty() const
{
    return terrains.empty();
}

std::size_t TmxTerrainTypes::size() const
{
    return terrains.size();
}

Err TmxTerrainTypes::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    foreachElement(elem, TmxTerrain::TAG_NAME, [&](Element const & e){
        TmxTerrain terrain;
        if (isSuccess(terrain.read(e))) {
            terrains.push_back(std::move(terrain));
        }
    });
    return E_SUCCESS;
}

Err TmxTerrainTypes::read(std::string const & xml)
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

Err TmxTerrainTypes::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    for (auto & terrain : terrains) {
        newElement(elem, TmxTerrain::TAG_NAME, [&](Element & p){
            terrain.write(p);
        });
    }
    return E_SUCCESS;
}

Err TmxTerrainTypes::write(std::string & xml) const
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

