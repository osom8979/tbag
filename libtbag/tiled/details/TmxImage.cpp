/**
 * @file   TmxImage.cpp
 * @brief  TmxImage class implementation.
 * @author zer0
 * @date   2019-07-24
 */

#include <libtbag/tiled/details/TmxImage.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxImage::TmxImage()
{
    // EMPTY.
}

TmxImage::TmxImage(std::string const & f, int i, std::string const & s, Color t, int w, int h)
        : format(f), id(i), source(s), trans(t), width(w), height(h)
{
    // EMPTY.
}

TmxImage::~TmxImage()
{
    // EMPTY.
}

Err TmxImage::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_FORMAT, format);
    optAttr(elem, ATT_ID, id);
    optAttr(elem, ATT_SOURCE, source);

    std::string trans_text;
    optAttr(elem, ATT_TRANS, trans_text);
    trans.fromRgb24String(trans_text);

    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);

    auto const * data_elem = elem.FirstChildElement(TmxData::TAG_NAME);
    if (data_elem != nullptr) {
        data.read(*data_elem);
    }

    return E_SUCCESS;
}

Err TmxImage::read(std::string const & xml)
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

Err TmxImage::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_FORMAT, format);
    setAttr(elem, ATT_ID, id);
    setAttr(elem, ATT_SOURCE, source);
    setAttr(elem, ATT_TRANS, trans.toRgb24String());
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);

    if (!data.empty()) {
        newElement(elem, TmxData::TAG_NAME, [&](Element & d){
            data.write(d);
        });
    }

    return E_SUCCESS;
}

Err TmxImage::write(std::string & xml) const
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

