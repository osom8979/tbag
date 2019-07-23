/**
 * @file   TmxFrame.cpp
 * @brief  TmxFrame class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxFrame.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxFrame::TmxFrame()
{
    // EMPTY.
}

TmxFrame::TmxFrame(int t, int d) : tileid(t), duration(d)
{
    // EMPTY.
}

TmxFrame::~TmxFrame()
{
    // EMPTY.
}

Err TmxFrame::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_TILEID, tileid);
    optAttr(elem, ATT_DURATION, duration);
    return E_SUCCESS;
}

Err TmxFrame::read(std::string const & xml)
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

Err TmxFrame::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_TILEID, tileid);
    setAttr(elem, ATT_DURATION, duration);
    return E_SUCCESS;
}

Err TmxFrame::write(std::string & xml) const
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

