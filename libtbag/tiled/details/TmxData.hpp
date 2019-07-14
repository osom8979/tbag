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
#include <libtbag/tiled/details/TmxDataCommon.hpp>
#include <libtbag/tiled/details/TmxChunk.hpp>

#include <string>
#include <vector>

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
 *  with the highest <code>firstgid</code> that is still lower or equal than the gid. The tilesets @n
 *  are always stored with increasing <code>firstgids</code>.
 */
struct TBAG_API TmxData : public TmxDataCommon
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "data";

    using GlobalIds = std::vector<std::uint32_t>;
    using Chunks = std::vector<TmxChunk>;

    Encoding encoding;
    Compression compression;

    enum class DataType
    {
        GIDS, CHUNK,
    };

    DataType data_type;

    /** The global tile IDs. */
    GlobalTileIds gids;

    /** Only for infinite maps. */
    std::vector<TmxChunk> chunks;

    TmxData();
    TmxData(Encoding e, Compression c, DataType d);
    ~TmxData();

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

