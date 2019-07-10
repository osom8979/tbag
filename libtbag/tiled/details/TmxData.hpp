/**
 * @file   TmxData.hpp
 * @brief  TmxData class prototype.
 * @author zer0
 * @date   2019-07-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxData class prototype.
 *
 * @author zer0
 * @date   2019-07-09
 *
 * @remarks
 *  When no encoding or compression is given, the tiles are stored as individual XML <code>tile</code> elements. @n
 *  Next to that, the easiest format to parse is the "csv" (comma separated values) format. @n
 *  @n
 *  The base64-encoded and optionally compressed layer data is somewhat more complicated to parse. @n
 *  First you need to base64-decode it, then you may need to decompress it. @n
 *  Now you have an array of bytes, which should be interpreted as an array of unsigned 32-bit integers @n
 *  using little-endian byte ordering. @n
 *  @n
 *  Whatever format you choose for your layer data, you will always end up with so called "global tile IDs" (gids). @n
 *  They are global, since they may refer to a tile from any of the tilesets used by the map. @n
 *  In order to find out from which tileset the tile is you need to find the tileset @n
 *  with the highest firstgid that is still lower or equal than the gid. The tilesets @n
 *  are always stored with increasing firstgids.
 */
struct TBAG_API TmxData : protected libtbag::dom::xml::XmlHelper
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "data";

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

    enum class Encoding
    {
        NONE, BASE64, CSV,
    };

    enum class Compression
    {
        NONE, GZIP, ZLIB,
    };

    Encoding encoding;
    Compression compression;
    std::string data;

    // std::vector<int> tile_gid; ///< The global tile IDs.

    TmxData();
    TmxData(Encoding e, Compression c, std::string const & data);
    ~TmxData();

    static Encoding getEncoding(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getEncodingName(Encoding e) TBAG_NOEXCEPT;

    static Compression getCompression(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getCompressionName(Compression c) TBAG_NOEXCEPT;

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXDATA_HPP__

