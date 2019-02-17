/**
 * @file   SfCommon.hpp
 * @brief  SfCommon class prototype.
 * @author zer0
 * @date   2019-02-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

#ifndef TBAG_SF_HANDLE_MAP
#define TBAG_SF_HANDLE_MAP(_TBAG_WINDOW_XX, _TBAG_GRAPHIC_XX) \
    /* Window API */                             \
    _TBAG_WINDOW_XX(CURSOR       , Cursor      ) \
    _TBAG_WINDOW_XX(RENDER_WINDOW, RenderWindow) \
    /* Graphic API */                                 \
    _TBAG_GRAPHIC_XX(VIEW           , View          ) \
    _TBAG_GRAPHIC_XX(TRANSFORM      , Transform     ) \
    _TBAG_GRAPHIC_XX(CIRCLE_SHAPE   , CircleShape   ) \
    _TBAG_GRAPHIC_XX(CONVEX_SHAPE   , ConvexShape   ) \
    _TBAG_GRAPHIC_XX(RECTANGLE_SHAPE, RectangleShape) \
    _TBAG_GRAPHIC_XX(SPRITE         , Sprite        ) \
    _TBAG_GRAPHIC_XX(TEXT           , Text          ) \
    _TBAG_GRAPHIC_XX(VERTEX_ARRAY   , VertexArray   ) \
    _TBAG_GRAPHIC_XX(VERTEX_BUFFER  , VertexBuffer  ) \
    /* -- END -- */
#endif

#ifndef TBAG_SF_HANDLE_MAP_ALL
#define TBAG_SF_HANDLE_MAP_ALL(_TBAG_XX) \
    TBAG_SF_HANDLE_MAP(_TBAG_XX, _TBAG_XX)
#endif

/**
 * Table of sfml types.
 *
 * @author zer0
 * @date   2019-02-12
 */
enum class SfType : int
{
    __START_NUMBER__ = -1,
#define _TBAG_XX(name, type) ST_##name,
    TBAG_SF_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    __SIZE__
};

TBAG_API SfType getSfType(int type) TBAG_NOEXCEPT;
TBAG_API char const * const getSfTypeName(SfType type) TBAG_NOEXCEPT;

TBAG_API void * newSfType(SfType type);
TBAG_API void deleteSfType(SfType type, void * ptr);

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFCOMMON_HPP__

