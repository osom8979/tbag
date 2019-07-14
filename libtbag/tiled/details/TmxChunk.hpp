/**
 * @file   TmxChunk.hpp
 * @brief  TmxChunk class prototype.
 * @author zer0
 * @date   2019-07-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXCHUNK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXCHUNK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/tiled/details/TmxDataCommon.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxChunk class prototype.
 *
 * @author zer0
 * @date   2019-07-10
 *
 * @remarks
 *  This is currently added only for infinite maps. @n
 *  The contents of a chunk element is same as that of the <code>data</code> element, @n
 *  except it stores the data of the area specified in the attributes.
 */
struct TBAG_API TmxChunk : public TmxDataCommon
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "chunk";

    /** The x coordinate of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_X = "x";

    /** The y coordinate of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_Y = "y";

    /** The width of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_WIDTH = "width";

    /** The height of the chunk in tiles. */
    TBAG_CONSTEXPR static char const * const ATT_HEIGHT = "height";

    int x;
    int y;
    int width;
    int height;

    GlobalTileIds gids;

    TmxChunk();
    TmxChunk(int x_, int y_, int w_, int h_);
    ~TmxChunk();

    Err read(Element const & elem, Encoding e, Compression c);
    Err read(std::string const & xml, Encoding e, Compression c);

    Err write(Element & elem, Encoding e, Compression c) const;
    Err write(std::string & xml, Encoding e, Compression c) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXCHUNK_HPP__

