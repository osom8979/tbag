/**
 * @file   TmxData.cpp
 * @brief  TmxData class implementation.
 * @author zer0
 * @date   2019-07-09
 */

#include <libtbag/tiled/details/TmxData.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxData::TmxData() : encoding(Encoding::NONE), compression(Compression::NONE), data()
{
    // EMPTY.
}

TmxData::TmxData(Encoding e, Compression c, std::string const & d)
        : encoding(e), compression(c), data(d)
{
    // EMPTY.
}

TmxData::~TmxData()
{
    // EMPTY.
}

TmxData::Encoding TmxData::getEncoding(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_BASE64) {
        return Encoding::BASE64;
    } else if (text == VAL_CSV) {
        return Encoding::CSV;
    } else {
        return Encoding::NONE;
    }
}

char const * const TmxData::getEncodingName(Encoding e) TBAG_NOEXCEPT
{
    // clang-format off
    switch (e) {
    case Encoding::BASE64:  return VAL_BASE64;
    case Encoding::CSV:     return VAL_CSV;
    case Encoding::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

TmxData::Compression TmxData::getCompression(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_GZIP) {
        return Compression::GZIP;
    } else if (text == VAL_ZLIB) {
        return Compression::ZLIB;
    } else {
        return Compression::NONE;
    }
}

char const * const TmxData::getCompressionName(Compression c) TBAG_NOEXCEPT
{
    // clang-format off
    switch (c) {
    case Compression::GZIP:  return VAL_GZIP;
    case Compression::ZLIB:  return VAL_ZLIB;
    case Compression::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

Err TmxData::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    std::string encoding_text;
    optAttr(elem, ATT_ENCODING, encoding_text);
    encoding = getEncoding(encoding_text);
    if (encoding == Encoding::NONE) {
        tDLogW("TmxData::read() Unknown Encoding value: {}", encoding_text);
    }

    std::string compression_text;
    optAttr(elem, ATT_COMPRESSION, compression_text);
    compression = getCompression(compression_text);
    if (compression == Compression::NONE) {
        tDLogW("TmxData::read() Unknown Compression value: {}", compression_text);
    }

    data = libtbag::string::trim(text(elem));

    return E_SUCCESS;
}

Err TmxData::read(std::string const & xml)
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

Err TmxData::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_ENCODING, getEncodingName(encoding));
    setAttr(elem, ATT_COMPRESSION, getCompressionName(compression));
    text(elem, data);
    return E_SUCCESS;
}

Err TmxData::write(std::string & xml) const
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

