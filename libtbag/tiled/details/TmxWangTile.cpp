/**
 * @file   TmxWangTile.cpp
 * @brief  TmxWangTile class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxWangTile.hpp>
#include <libtbag/string/StringUtils.hpp>

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

TmxWangTile::TmxWangTile(int t, int w) : tileid(t), wangid(w)
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

//    std::string wangid_text;
//    auto code = optAttr(elem, ATT_WANGID, wangid_text);
//    if (isSuccess(code)) {
//        using namespace libtbag::string;
//        using namespace libtbag::util;
//        Buffer buffer;
//        code = convertHexStringToBuffer(wangid_text.c_str(), wangid_text.size(), buffer);
//        if (isSuccess(code)) {
//            // &buffer[0]
//        } else {
//            return code;
//        }
//    } else {
//        return code;
//    }

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
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

