/**
 * @file   TmxWangTile.cpp
 * @brief  TmxWangTile class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxWangTile.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxWangTile::TmxWangTile()
{
    // EMPTY.
}

TmxWangTile::TmxWangTile(int t, WangId w) : tileid(t), wangid(w)
{
    // EMPTY.
}

TmxWangTile::~TmxWangTile()
{
    // EMPTY.
}

Err TmxWangTile::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_TILEID, tileid);

    std::string wangid_text;
    auto const code = optAttr(elem, ATT_WANGID, wangid_text);
    if (isSuccess(code)) {
        if (!libtbag::string::toVal(wangid_text, wangid, nullptr, 16)) {
            return E_PARSING;
        }
    } else {
        return code;
    }

    return E_SUCCESS;
}

Err TmxWangTile::read(std::string const & xml)
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

Err TmxWangTile::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_TILEID, tileid);
    setAttr(elem, ATT_WANGID, libtbag::string::fformat("0x{:0>8X}", wangid));
    return E_SUCCESS;
}

Err TmxWangTile::write(std::string & xml) const
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

int TmxWangTile::getCornerRight() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0xF0000000) >> 28);
}

int TmxWangTile::getCornerBottom() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x00F00000) >> 20);
}

int TmxWangTile::getCornerLeft() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x0000F000) >> 12);
}

int TmxWangTile::getCornerTop() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x000000F0) >> 4);
}

int TmxWangTile::getEdgeTopRight() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x0F000000) >> 24);
}

int TmxWangTile::getEdgeBottomRight() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x000F0000) >> 16);
}

int TmxWangTile::getEdgeBottomLeft() const TBAG_NOEXCEPT
{
    return static_cast<int>((wangid & 0x00000F00) >> 8);
}

int TmxWangTile::getEdgeTopLeft() const TBAG_NOEXCEPT
{
    return static_cast<int>(wangid & 0x0000000F);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

