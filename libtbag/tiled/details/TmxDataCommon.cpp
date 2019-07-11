/**
 * @file   TmxDataCommon.cpp
 * @brief  TmxDataCommon class implementation.
 * @author zer0
 * @date   2019-07-10
 */

#include <libtbag/tiled/details/TmxDataCommon.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/crypto/Base64.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxDataCommon::TmxDataCommon()
{
    // EMPTY.
}

TmxDataCommon::~TmxDataCommon()
{
    // EMPTY.
}

TmxDataCommon::Encoding TmxDataCommon::getEncoding(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_BASE64) {
        return Encoding::BASE64;
    } else if (text == VAL_CSV) {
        return Encoding::CSV;
    } else {
        return Encoding::NONE;
    }
}

char const * const TmxDataCommon::getEncodingName(Encoding e) TBAG_NOEXCEPT
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

TmxDataCommon::Compression TmxDataCommon::getCompression(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_GZIP) {
        return Compression::GZIP;
    } else if (text == VAL_ZLIB) {
        return Compression::ZLIB;
    } else {
        return Compression::NONE;
    }
}

char const * const TmxDataCommon::getCompressionName(Compression c) TBAG_NOEXCEPT
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

std::string TmxDataCommon::writeToBase64(GlobalTileId const * guis, std::size_t size)
{
    return "";
}

std::string TmxDataCommon::writeToBase64Gzip(GlobalTileId const * guis, std::size_t size)
{
    return "";
}

std::string TmxDataCommon::writeToBase64Zlib(GlobalTileId const * guis, std::size_t size)
{
    return "";
}

std::string TmxDataCommon::writeToCsv(GlobalTileId const * guis, std::size_t size)
{
    return "";
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromBase64(std::string const & text)
{
    if (text.empty()) {
        return {};
    }

    using namespace libtbag::util;
    using namespace libtbag::crypto;

    Buffer buffer;
    if (!decodeBase64(text, buffer)) {
        return {};
    }

    auto const SIZE = buffer.size();
    if ((SIZE%4) != 0) {
        return {};
    }

    auto const * data = (GlobalTileId const *)(buffer.data());
    assert(data != nullptr);

    if (libtbag::bitwise::isLittleEndianSystem()) {
        return GlobalTileIds(data, data + (SIZE/4));
    } else {
        assert(libtbag::bitwise::isBigEndianSystem());
        auto const ID_SIZE = SIZE/4;
        GlobalTileIds result(ID_SIZE);
        for (auto i = 0; i < ID_SIZE; ++i) {
            result[i] = libtbag::bitwise::toHost(*data);
        }
        return result;
    }
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromBase64Gzip(std::string const & text)
{
    return {};
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromBase64Zlib(std::string const & text)
{
    return {};
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromCsv(std::string const & text)
{
    return {};
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

