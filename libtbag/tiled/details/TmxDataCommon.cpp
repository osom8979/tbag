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
#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/string/StringUtils.hpp>

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

std::string TmxDataCommon::writeToBase64(GlobalTileId const * gids, std::size_t size)
{
    assert(gids != nullptr);
    assert(size >= 1);
    assert(libtbag::bitwise::isLittleEndianSystem());

    std::string result;
    if (!libtbag::crypto::encodeBase64((char const *)gids, size*sizeof(GlobalTileId), result)) {
        return {};
    }
    return result;
}

std::string TmxDataCommon::writeToCompressedBase64(GlobalTileId const * gids, std::size_t size, Compression c)
{
    assert(gids != nullptr);
    assert(size >= 1);
    assert(libtbag::bitwise::isLittleEndianSystem());
    assert(c != Compression::NONE);

    using namespace libtbag::archive;
    using namespace libtbag::archive::ex;
    auto const METHOD = (c == Compression::GZIP ? CompressionMethod::CM_GZIP : CompressionMethod::CM_ZLIB);

    std::string result;
    auto const code = encodeZipBase64((char const *)gids, size*sizeof(GlobalTileId), result,
                                      TBAG_ZIP_DEFAULT_ENCODE_LEVEL, METHOD);
    if (isFailure(code)) {
        return {};
    }
    return result;
}

std::string TmxDataCommon::writeToGzipBase64(GlobalTileId const * gids, std::size_t size)
{
    return writeToCompressedBase64(gids, size, Compression::GZIP);
}

std::string TmxDataCommon::writeToZlibBase64(GlobalTileId const * gids, std::size_t size)
{
    return writeToCompressedBase64(gids, size, Compression::ZLIB);
}

std::string TmxDataCommon::writeToCsv(GlobalTileId const * gids, std::size_t size)
{
    if (gids == nullptr || size == 0) {
        return {};
    }
    std::vector<std::string> tokens(size);
    for (auto i = 0; i < size; ++i) {
        tokens[i] = libtbag::string::toString(gids[i]);
    }
    return libtbag::string::mergeTokens(tokens, CSV_DELIMITER);
}

TmxDataCommon::GlobalTileIds TmxDataCommon::convertGlobalTileIds(Buffer const & buffer)
{
    if (buffer.empty()) {
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

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromBase64(std::string const & text)
{
    using namespace libtbag::crypto;
    if (text.empty()) {
        return {};
    }
    Buffer buffer;
    if (!decodeBase64(text, buffer)) {
        return {};
    }
    return convertGlobalTileIds(buffer);
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromCompressedBase64(std::string const & text)
{
    using namespace libtbag::archive::ex;
    if (text.empty()) {
        return {};
    }
    Buffer buffer;
    if (isFailure(decodeZipBase64(text, buffer))) {
        return {};
    }
    return convertGlobalTileIds(buffer);
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromGzipBase64(std::string const & text)
{
    return readFromCompressedBase64(text);
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromZlibBase64(std::string const & text)
{
    return readFromCompressedBase64(text);
}

TmxDataCommon::GlobalTileIds TmxDataCommon::readFromCsv(std::string const & text)
{
    auto const tokens = libtbag::string::splitTokens(text, CSV_DELIMITER, false);
    auto const size = tokens.size();
    GlobalTileIds result(size);
    for (auto i = 0; i < size; ++i) {
        result[i] = libtbag::string::toValue<GlobalTileId>(tokens[i]);
    }
    return result;
}

Err TmxDataCommon::readGids(Element const & elem, GlobalTileIds & gids, Encoding e, Compression c)
{
    if (e == Encoding::CSV) {
        if (c != Compression::NONE) {
            return E_ILLARGS;
        }
        gids = readFromCsv(text(elem));

    } else if (e == Encoding::BASE64) {
        if (c == Compression::GZIP) {
            gids = readFromGzipBase64(text(elem));
        } else if (c == Compression::ZLIB) {
            gids = readFromZlibBase64(text(elem));
        } else {
            assert(c == Compression::NONE);
            gids = readFromBase64(text(elem));
        }

    } else if (e == Encoding::NONE) {
        if (c != Compression::NONE) {
            return E_ILLARGS;
        }
        gids.clear();
        foreachElement(elem, TAG_TILE, [&](Element const & tile){
            std::string gid_text;
            if (isSuccess(optAttr(tile, ATT_GID, gid_text))) {
                gids.push_back(libtbag::string::toValue<int>(gid_text));
            } else {
                gids.push_back(0);
            }
        });
    }

    return gids.empty() ? E_DECODE : E_SUCCESS;
}

Err TmxDataCommon::writeGids(Element & elem, GlobalTileId const * gids, std::size_t size, Encoding e, Compression c)
{
    assert(gids != nullptr);
    assert(size >= 1);

    if (e == Encoding::CSV) {
        if (c != Compression::NONE) {
            return E_ILLARGS;
        }
        auto const cdata = writeToCsv(gids, size);
        if (cdata.empty()) {
            return E_ENCODE;
        } else {
            text(elem, cdata);
            return E_SUCCESS;
        }

    } else if (e == Encoding::BASE64) {
        std::string cdata;
        if (c == Compression::GZIP) {
            cdata = writeToGzipBase64(gids, size);
        } else if (c == Compression::ZLIB) {
            cdata = writeToZlibBase64(gids, size);
        } else {
            assert(c == Compression::NONE);
            cdata = writeToBase64(gids, size);
        }
        if (cdata.empty()) {
            return E_ENCODE;
        } else {
            text(elem, cdata);
            return E_SUCCESS;
        }
    }

    assert(e == Encoding::NONE);
    if (c != Compression::NONE) {
        return E_ILLARGS;
    }
    for (auto i = 0; i < size; ++i) {
        newElement(elem, TAG_TILE, [&](Element & tile){
            if (gids[i]) {
                setAttr(tile, ATT_GID, gids[i]);
            }
        });
    }
    return E_SUCCESS;
}

Err TmxDataCommon::writeGids(Element & elem, GlobalTileIds const & gids, Encoding e, Compression c)
{
    return writeGids(elem, gids.data(), gids.size(), e, c);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

