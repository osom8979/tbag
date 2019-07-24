/**
 * @file   TmxProperties.cpp
 * @brief  TmxProperties class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxProperties::TmxProperties()
{
    // EMPTY.
}

TmxProperties::TmxProperties(Properties const & p) : properties(p)
{
    // EMPTY.
}

TmxProperties::~TmxProperties()
{
    // EMPTY.
}

bool TmxProperties::empty() const
{
    return properties.empty();
}

std::size_t TmxProperties::size() const
{
    return properties.size();
}

Err TmxProperties::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    foreachElement(elem, TmxProperty::TAG_NAME, [&](Element const & e){
        TmxProperty property;
        if (isSuccess(property.read(e))) {
            properties.push_back(std::move(property));
        }
    });
    return E_SUCCESS;
}

Err TmxProperties::read(std::string const & xml)
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

Err TmxProperties::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    for (auto & property : properties) {
        newElement(elem, TmxProperty::TAG_NAME, [&](Element & p){
            property.write(p);
        });
    }
    return E_SUCCESS;
}

Err TmxProperties::write(std::string & xml) const
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

