/**
 * @file   TmxFrame.hpp
 * @brief  TmxFrame class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXFRAME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/tiled/details/TmxTag.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxFrame class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxFrame : public TmxTag
{
    TBAG_CONSTEXPR static char const * const TAG_NAME = "frame";

    /** The local ID of a tile within the parent <code>tileset</code>. */
    TBAG_CONSTEXPR static char const * const ATT_TILEID = "tileid";

    /** How long (in milliseconds) this frame should be displayed before advancing to the next frame. */
    TBAG_CONSTEXPR static char const * const ATT_DURATION = "duration";

    int tileid = 0;
    int duration = 0;

    TmxFrame();
    TmxFrame(int t, int d);
    virtual ~TmxFrame();

    virtual Err onRead(Element const & elem) override;
    virtual Err onWrite(Element & elem) const override;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXFRAME_HPP__

