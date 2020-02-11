/**
 * @file   TmxWangSets.cpp
 * @brief  TmxWangSets class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxWangSets.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxWangSets::TmxWangSets()
{
    // EMPTY.
}

TmxWangSets::TmxWangSets(WangSets const & w) : wang_sets(w)
{
    // EMPTY.
}

TmxWangSets::~TmxWangSets()
{
    // EMPTY.
}

bool TmxWangSets::empty() const
{
    return wang_sets.empty();
}

std::size_t TmxWangSets::size() const
{
    return wang_sets.size();
}

void TmxWangSets::clear()
{
    wang_sets.clear();
}

Err TmxWangSets::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    foreachElement(elem, TmxWangSet::TAG_NAME, [&](Element const & e){
        TmxWangSet wang_set;
        if (isSuccess(wang_set.read(e))) {
            wang_sets.push_back(std::move(wang_set));
        }
    });
    return E_SUCCESS;
}

Err TmxWangSets::read(std::string const & xml)
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

Err TmxWangSets::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    for (auto & wang_set : wang_sets) {
        newElement(elem, TmxWangSet::TAG_NAME, [&](Element & p){
            wang_set.write(p);
        });
    }
    return E_SUCCESS;
}

Err TmxWangSets::write(std::string & xml) const
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

