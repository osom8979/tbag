/**
 * @file   SfCommon.hpp
 * @brief  SfCommon class prototype.
 * @author zer0
 * @date   2019-02-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_COMMON_HPP__

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
#define TBAG_SF_HANDLE_MAP(_TBAG_PRIMITIVE_XX, _TBAG_DRAWABLE_XX, _TBAG_RENDER_TARGET_XX) \
    /* Window API */                   \
    _TBAG_PRIMITIVE_XX(CURSOR, Cursor) \
    /* Graphic API */                               \
    _TBAG_PRIMITIVE_XX(BLEND_MODE   , BlendMode   ) \
    _TBAG_PRIMITIVE_XX(IMAGE        , Image       ) \
    _TBAG_PRIMITIVE_XX(RENDER_STATES, RenderStates) \
    _TBAG_PRIMITIVE_XX(SHADER       , Shader      ) \
    _TBAG_PRIMITIVE_XX(TEXTURE      , Texture     ) \
    _TBAG_PRIMITIVE_XX(VIEW         , View        ) \
    _TBAG_PRIMITIVE_XX(TRANSFORM    , Transform   ) \
    /* Drawable API */                                 \
    _TBAG_DRAWABLE_XX(CIRCLE_SHAPE   , CircleShape   ) \
    _TBAG_DRAWABLE_XX(CONVEX_SHAPE   , ConvexShape   ) \
    _TBAG_DRAWABLE_XX(RECTANGLE_SHAPE, RectangleShape) \
    _TBAG_DRAWABLE_XX(SPRITE         , Sprite        ) \
    _TBAG_DRAWABLE_XX(TEXT           , Text          ) \
    _TBAG_DRAWABLE_XX(VERTEX_ARRAY   , VertexArray   ) \
    _TBAG_DRAWABLE_XX(VERTEX_BUFFER  , VertexBuffer  ) \
    /* RenderTarget API */                              \
    _TBAG_RENDER_TARGET_XX(RENDER_WINDOW, RenderWindow) \
    /* -- END -- */
#endif

#ifndef TBAG_SF_HANDLE_MAP_ALL
#define TBAG_SF_HANDLE_MAP_ALL(_TBAG_XX) \
    TBAG_SF_HANDLE_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
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
TBAG_API bool isRenderTarget(SfType type) TBAG_NOEXCEPT;

TBAG_API void * newSfType(SfType type);
TBAG_API void deleteSfType(SfType type, void * ptr);

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_COMMON_HPP__

