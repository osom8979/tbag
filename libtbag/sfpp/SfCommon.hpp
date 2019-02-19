/**
 * @file   SfCommon.hpp
 * @brief  SfCommon class prototype.
 * @author zer0
 * @date   2019-02-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFCOMMON_HPP__

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

namespace sfpp {

#ifndef TBAG_SF_HANDLE_MAP
#define TBAG_SF_HANDLE_MAP(_TBAG_WINDOW_XX, _TBAG_GRAPHIC_XX) \
    /* Window API */                \
    _TBAG_WINDOW_XX(CURSOR, Cursor) \
    /* Graphic API */                                 \
    _TBAG_GRAPHIC_XX(BLEND_MODE     , BlendMode     ) \
    _TBAG_GRAPHIC_XX(IMAGE          , Image         ) \
    _TBAG_GRAPHIC_XX(RENDER_STATES  , RenderStates  ) \
    _TBAG_GRAPHIC_XX(SHADER         , Shader        ) \
    _TBAG_GRAPHIC_XX(TEXTURE        , Texture       ) \
    _TBAG_GRAPHIC_XX(VIEW           , View          ) \
    _TBAG_GRAPHIC_XX(TRANSFORM      , Transform     ) \
    _TBAG_GRAPHIC_XX(CIRCLE_SHAPE   , CircleShape   ) \
    _TBAG_GRAPHIC_XX(CONVEX_SHAPE   , ConvexShape   ) \
    _TBAG_GRAPHIC_XX(RECTANGLE_SHAPE, RectangleShape) \
    _TBAG_GRAPHIC_XX(SPRITE         , Sprite        ) \
    _TBAG_GRAPHIC_XX(TEXT           , Text          ) \
    _TBAG_GRAPHIC_XX(VERTEX_ARRAY   , VertexArray   ) \
    _TBAG_GRAPHIC_XX(VERTEX_BUFFER  , VertexBuffer  ) \
    _TBAG_GRAPHIC_XX(RENDER_WINDOW  , RenderWindow  ) \
    /* Not implementation */            \
    /* (DRAWABLE     , Drawable    ) */ \
    /* (RENDER_TARGET, RenderTarget) */ \
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

TBAG_API bool isDrawable(SfType type) TBAG_NOEXCEPT;

TBAG_API void * newSfType(SfType type);
TBAG_API void deleteSfType(SfType type, void * ptr);

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFCOMMON_HPP__

