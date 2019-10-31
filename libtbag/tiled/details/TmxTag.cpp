/**
 * @file   TmxTag.cpp
 * @brief  TmxTag class implementation.
 * @author zer0
 * @date   2019-08-17
 */

#include <libtbag/tiled/details/TmxTag.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTag::TmxTag(char const * tag) : TAG(tag)
{
    // EMPTY.
}

TmxTag::~TmxTag()
{
    // EMPTY.
}

Err TmxTag::onRead(Element const & elem)
{
    return E_ENOSYS;
}

Err TmxTag::onWrite(Element & elem) const
{
    return E_ENOSYS;
}

bool TmxTag::validateTagName(Element const & elem) const
{
    return ::strcmp(elem.Name(), TAG) == 0;
}

Err TmxTag::read(Element const & elem)
{
    if (!validateTagName(elem)) {
        return E_ILLARGS;
    }
    return onRead(elem);
}

Err TmxTag::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxTag::write(Element & elem) const
{
    if (!validateTagName(elem)) {
        return E_ILLARGS;
    }
    return onWrite(elem);
}

Err TmxTag::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG);
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

