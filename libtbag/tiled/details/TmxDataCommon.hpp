/**
 * @file   TmxDataCommon.hpp
 * @brief  TmxDataCommon class prototype.
 * @author zer0
 * @date   2019-07-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATACOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATACOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxDataCommon class prototype.
 *
 * @author zer0
 * @date   2019-07-10
 */
struct TBAG_API TmxDataCommon : protected libtbag::dom::xml::XmlHelper
{
    /**
     * The encoding used to encode the tile layer data.
     * When used, it can be "base64" and "csv" at the moment.
     */
    TBAG_CONSTEXPR static char const * const ATT_ENCODING = "encoding";
    TBAG_CONSTEXPR static char const * const VAL_BASE64 = "base64";
    TBAG_CONSTEXPR static char const * const VAL_CSV = "csv";

    /**
     * The compression used to compress the tile layer data.
     * Tiled supports "gzip" and "zlib".
     */
    TBAG_CONSTEXPR static char const * const ATT_COMPRESSION = "compression";
    TBAG_CONSTEXPR static char const * const VAL_GZIP = "gzip";
    TBAG_CONSTEXPR static char const * const VAL_ZLIB = "zlib";

    /** Array of unsigned 32-bit integers using little-endian byte ordering. */
    using GlobalTileId  = std::uint32_t;
    using GlobalTileIds = std::vector<GlobalTileId>;

    enum class Encoding
    {
        NONE, BASE64, CSV,
    };

    enum class Compression
    {
        NONE, GZIP, ZLIB,
    };

    TmxDataCommon();
    ~TmxDataCommon();

    static Encoding getEncoding(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getEncodingName(Encoding e) TBAG_NOEXCEPT;

    static Compression getCompression(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getCompressionName(Compression c) TBAG_NOEXCEPT;

    static std::string writeToBase64    (GlobalTileId const * guis, std::size_t size);
    static std::string writeToBase64Gzip(GlobalTileId const * guis, std::size_t size);
    static std::string writeToBase64Zlib(GlobalTileId const * guis, std::size_t size);
    static std::string writeToCsv       (GlobalTileId const * guis, std::size_t size);

    static GlobalTileIds readFromBase64    (std::string const & text);
    static GlobalTileIds readFromBase64Gzip(std::string const & text);
    static GlobalTileIds readFromBase64Zlib(std::string const & text);
    static GlobalTileIds readFromCsv       (std::string const & text);
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATACOMMON_HPP__

