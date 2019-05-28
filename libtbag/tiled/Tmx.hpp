/**
 * @file   Tmx.hpp
 * @brief  Tmx class prototype.
 * @author zer0
 * @date   2019-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

/**
 * Tmx class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 *
 * @see <https://doc.mapeditor.org/en/stable/reference/tmx-map-format/>
 */
class TBAG_API Tmx
{
public:
    Tmx();
    Tmx(Tmx const & obj);
    Tmx(Tmx && obj) TBAG_NOEXCEPT;
    ~Tmx();

public:
    Tmx & operator =(Tmx const & obj);
    Tmx & operator =(Tmx && obj) TBAG_NOEXCEPT;

public:
    void copy(Tmx const & obj);
    void swap(Tmx & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Tmx & lh, Tmx & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_TMX_HPP__

