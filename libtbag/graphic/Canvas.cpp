/**
 * @file   Canvas.cpp
 * @brief  Canvas class implementation.
 * @author zer0
 * @date   2020-01-11
 */

#include <libtbag/graphic/Canvas.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#include <cassert>
#include <algorithm>
#include <utility>
#include <type_traits>

#include <blend2d.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

using Point2d = Canvas::Point2d;
using Point2i = Canvas::Point2i;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE2(BLPoint, x, y, Point2d, x, y);
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE2(BLPointI, x, y, Point2i, x, y);

using Rect2d = Canvas::Rect2d;
using Rect2i = Canvas::Rect2i;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE4(BLRect, x, y, w, h, Rect2d, x, y, width, height);
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE4(BLRectI, x, y, w, h, Rect2i, x, y, width, height);

using Size2d = Canvas::Size2d;
using Size2i = Canvas::Size2i;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE2(BLSize, w, h, Size2d, width, height);
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE2(BLSizeI, w, h, Size2i, width, height);

using Arc2d = Canvas::Arc2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE6(BLArc, cx, cy, rx, ry, start, sweep,
                                             Arc2d, cx, cy, rx, ry, start, sweep);
using Circle2d = Canvas::Circle2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE3(BLCircle, cx, cy, r,
                                             Circle2d, cx, cy, r);
using Ellipse2d = Canvas::Ellipse2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE4(BLEllipse, cx, cy, rx, ry,
                                             Ellipse2d, cx, cy, rx, ry);
using Line2d = Canvas::Line2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE4(BLLine, x0, y0, x1, y1,
                                             Line2d, x0, y0, x1, y1);
using RoundRect2d = Canvas::RoundRect2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE6(BLRoundRect, x, y, w, h, rx, ry,
                                             RoundRect2d, x, y, width, height, rx, ry);
using Triangle2d = Canvas::Triangle2d;
STATIC_ASSERT_IS_REINTERPRET_CAST_STRUCTURE6(BLTriangle, x0, y0, x1, y1, x2, y2,
                                             Triangle2d, x0, y0, x1, y1, x2, y2);

using ContextType = Canvas::ContextType;
using BlendMode = Canvas::BlendMode;
using ExtendMode = Canvas::ExtendMode;
using GeometryDirection = Canvas::GeometryDirection;
using Matrix = Canvas::Matrix;
using FontMatrix = Canvas::FontMatrix;
using FontMetrics = Canvas::FontMetrics;
using FontDesignMetrics = Canvas::FontDesignMetrics;
using GlyphRun = Canvas::GlyphRun;
using Box = Canvas::Box;
using Range = Canvas::Range;
using GradientStop = Canvas::GradientStop;
using LinearGradientValues = Canvas::LinearGradientValues;
using RadialGradientValues = Canvas::RadialGradientValues;
using ConicalGradientValues = Canvas::ConicalGradientValues;

// Tbag -> Blend2D

static BLPoint _cvt(Point2d const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static BLPointI _cvt(Point2i const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static BLRect _cvt(Rect2d const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.width, v.height};
}

static BLRectI _cvt(Rect2i const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.width, v.height};
}

static BLSize _cvt(Size2d const & v) TBAG_NOEXCEPT
{
    return {v.width, v.height};
}

static BLSizeI _cvt(Size2i const & v) TBAG_NOEXCEPT
{
    return {v.width, v.height};
}

static BLRgba32 _cvt(Rgb32 const & v) TBAG_NOEXCEPT
{
    return {v.r, v.g, v.b, v.a};
}

static BLRgba32 _cvt(Rgb24 const & v) TBAG_NOEXCEPT
{
    return {v.r, v.g, v.b, 0xFF};
}

static BLMatrix2D _cvt(Matrix const & v) TBAG_NOEXCEPT
{
    return {v.array[0], v.array[1], v.array[2], v.array[3], v.array[4], v.array[5]};
}

static BLFontMatrix _cvt(FontMatrix const & v) TBAG_NOEXCEPT
{
    return {v.array[0], v.array[1], v.array[2], v.array[3]};
}

static BLRange _cvt(Range const & v) TBAG_NOEXCEPT
{
    return BLRange(v.start, v.end);
}

static BLGradientStop _cvt(GradientStop const & v) TBAG_NOEXCEPT
{
    return BLGradientStop(v.offset, BLRgba64(v.rgba.r, v.rgba.g, v.rgba.b, v.rgba.a));
}

static BLLinearGradientValues _cvt(LinearGradientValues const & v) TBAG_NOEXCEPT
{
    return BLLinearGradientValues(v.x0, v.y0, v.x1, v.y1);
}

static BLRadialGradientValues _cvt(RadialGradientValues const & v) TBAG_NOEXCEPT
{
    return BLRadialGradientValues(v.x0, v.y0, v.x1, v.y1, v.r0);
}

static BLConicalGradientValues _cvt(ConicalGradientValues const & v) TBAG_NOEXCEPT
{
    return BLConicalGradientValues(v.x0, v.y0, v.angle);
}

static BLFontMetrics _cvt(FontMetrics const & v) TBAG_NOEXCEPT
{
    BLFontMetrics result = {};
    // clang-format on
    result.size                   = v.size;
    result.ascent                 = v.ascent;
    result.vAscent                = v.vAscent;
    result.descent                = v.descent;
    result.vDescent               = v.vDescent;
    result.lineGap                = v.lineGap;
    result.xHeight                = v.xHeight;
    result.capHeight              = v.capHeight;
    result.underlinePosition      = v.underlinePosition;
    result.underlineThickness     = v.underlineThickness;
    result.strikethroughPosition  = v.strikethroughPosition;
    result.strikethroughThickness = v.strikethroughThickness;
    // clang-format off
    return result;
}

static BLFontDesignMetrics _cvt(FontDesignMetrics const & v) TBAG_NOEXCEPT
{
    BLFontDesignMetrics result = {};
    // clang-format on
    result.unitsPerEm             = v.unitsPerEm;
    result.lineGap                = v.lineGap;
    result.xHeight                = v.xHeight;
    result.capHeight              = v.capHeight;
    result.ascent                 = v.ascent;
    result.vAscent                = v.vAscent;
    result.descent                = v.descent;
    result.vDescent               = v.vDescent;
    result.hMinLSB                = v.hMinLSB;
    result.vMinLSB                = v.vMinLSB;
    result.hMinTSB                = v.hMinTSB;
    result.vMinTSB                = v.vMinTSB;
    result.hMaxAdvance            = v.hMaxAdvance;
    result.vMaxAdvance            = v.vMaxAdvance;
    result.underlinePosition      = v.underlinePosition;
    result.underlineThickness     = v.underlineThickness;
    result.strikethroughPosition  = v.strikethroughPosition;
    result.strikethroughThickness = v.strikethroughThickness;
    // clang-format off
    return result;
}

static BLGlyphRun _cvt(GlyphRun const & v) TBAG_NOEXCEPT
{
    BLGlyphRun result = {};
    // clang-format on
    result.glyphIdData      = v.glyphIdData;
    result.placementData    = v.placementData;
    result.size             = v.size;
    result.glyphIdSize      = v.glyphIdSize;
    result.placementType    = v.placementType;
    result.glyphIdAdvance   = v.glyphIdAdvance;
    result.placementAdvance = v.placementAdvance;
    result.flags            = v.flags;
    // clang-format off
    return result;
}

// Blend2D -> Tbag

static Point2d _cvt(BLPoint const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static Point2i _cvt(BLPointI const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static Rect2d _cvt(BLRect const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.w, v.h};
}

static Rect2i _cvt(BLRectI const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.w, v.h};
}

static Size2d _cvt(BLSize const & v) TBAG_NOEXCEPT
{
    return {v.w, v.h};
}

static Size2i _cvt(BLSizeI const & v) TBAG_NOEXCEPT
{
    return {v.w, v.h};
}

static Rgb32 _cvt(BLRgba32 const & v) TBAG_NOEXCEPT
{
    return Rgb32(v.r, v.g, v.b, v.a);
}

static Matrix _cvt(BLMatrix2D const & v) TBAG_NOEXCEPT
{
    return {v.m00, v.m01, v.m10, v.m11, v.m20, v.m21};
}

static FontMatrix _cvt(BLFontMatrix const & v) TBAG_NOEXCEPT
{
    return {v.m00, v.m01, v.m10, v.m11};
}

static Range _cvt(BLRange const & v) TBAG_NOEXCEPT
{
    return {v.start, v.end};
}

static GradientStop _cvt(BLGradientStop const & v) TBAG_NOEXCEPT
{
    return {v.offset, Rgb32(v.rgba.r, v.rgba.g, v.rgba.b, v.rgba.a)};
}

static LinearGradientValues _cvt(BLLinearGradientValues const & v) TBAG_NOEXCEPT
{
    return {v.x0, v.y0, v.x1, v.y1};
}

static RadialGradientValues _cvt(BLRadialGradientValues const & v) TBAG_NOEXCEPT
{
    return {v.x0, v.y0, v.x1, v.y1, v.r0};
}

static ConicalGradientValues _cvt(BLConicalGradientValues const & v) TBAG_NOEXCEPT
{
    return {v.x0, v.y0, v.angle};
}

static FontMetrics _cvt(BLFontMetrics const & v) TBAG_NOEXCEPT
{
    FontMetrics result = {};
    // clang-format on
    result.size                   = v.size;
    result.ascent                 = v.ascent;
    result.vAscent                = v.vAscent;
    result.descent                = v.descent;
    result.vDescent               = v.vDescent;
    result.lineGap                = v.lineGap;
    result.xHeight                = v.xHeight;
    result.capHeight              = v.capHeight;
    result.underlinePosition      = v.underlinePosition;
    result.underlineThickness     = v.underlineThickness;
    result.strikethroughPosition  = v.strikethroughPosition;
    result.strikethroughThickness = v.strikethroughThickness;
    // clang-format off
    return result;
}

static FontDesignMetrics _cvt(BLFontDesignMetrics const & v) TBAG_NOEXCEPT
{
    FontDesignMetrics result = {};
    // clang-format on
    result.unitsPerEm             = v.unitsPerEm;
    result.lineGap                = v.lineGap;
    result.xHeight                = v.xHeight;
    result.capHeight              = v.capHeight;
    result.ascent                 = v.ascent;
    result.vAscent                = v.vAscent;
    result.descent                = v.descent;
    result.vDescent               = v.vDescent;
    result.hMinLSB                = v.hMinLSB;
    result.vMinLSB                = v.vMinLSB;
    result.hMinTSB                = v.hMinTSB;
    result.vMinTSB                = v.vMinTSB;
    result.hMaxAdvance            = v.hMaxAdvance;
    result.vMaxAdvance            = v.vMaxAdvance;
    result.underlinePosition      = v.underlinePosition;
    result.underlineThickness     = v.underlineThickness;
    result.strikethroughPosition  = v.strikethroughPosition;
    result.strikethroughThickness = v.strikethroughThickness;
    // clang-format off
    return result;
}

static GlyphRun _cvt(BLGlyphRun const & v) TBAG_NOEXCEPT
{
    GlyphRun result = {};
    // clang-format on
    result.glyphIdData      = v.glyphIdData;
    result.placementData    = v.placementData;
    result.size             = v.size;
    result.glyphIdSize      = v.glyphIdSize;
    result.placementType    = v.placementType;
    result.glyphIdAdvance   = v.glyphIdAdvance;
    result.placementAdvance = v.placementAdvance;
    result.flags            = v.flags;
    // clang-format off
    return result;
}

template <typename BLType, typename TbagT>
struct BLStructureConverterImpl
{
    using bltype = BLType;
    using tbtype = TbagT;

    // clang-format off
    static bltype       * cast(tbtype       * p) TBAG_NOEXCEPT { return   (bltype       *) p ; }
    static bltype const * cast(tbtype const * p) TBAG_NOEXCEPT { return   (bltype const *) p ; }
    static bltype       & cast(tbtype       & p) TBAG_NOEXCEPT { return *((bltype       *)&p); }
    static bltype const & cast(tbtype const & p) TBAG_NOEXCEPT { return *((bltype const *)&p); }
    static tbtype       * cast(bltype       * p) TBAG_NOEXCEPT { return   (tbtype       *) p ; }
    static tbtype const * cast(bltype const * p) TBAG_NOEXCEPT { return   (tbtype const *) p ; }
    static tbtype       & cast(bltype       & p) TBAG_NOEXCEPT { return *((tbtype       *)&p); }
    static tbtype const & cast(bltype const & p) TBAG_NOEXCEPT { return *((tbtype const *)&p); }
    // clang-format on
};

template <typename T> struct _BLCast;

// clang-format off
template <> struct _BLCast<BLPoint    > : public BLStructureConverterImpl<BLPoint    , Point2d    > { using type = tbtype; };
template <> struct _BLCast<BLPointI   > : public BLStructureConverterImpl<BLPointI   , Point2i    > { using type = tbtype; };
template <> struct _BLCast<BLRect     > : public BLStructureConverterImpl<BLRect     , Rect2d     > { using type = tbtype; };
template <> struct _BLCast<BLRectI    > : public BLStructureConverterImpl<BLRectI    , Rect2i     > { using type = tbtype; };
template <> struct _BLCast<BLSize     > : public BLStructureConverterImpl<BLSize     , Size2d     > { using type = tbtype; };
template <> struct _BLCast<BLSizeI    > : public BLStructureConverterImpl<BLSizeI    , Size2i     > { using type = tbtype; };
template <> struct _BLCast<BLArc      > : public BLStructureConverterImpl<BLArc      , Arc2d      > { using type = tbtype; };
template <> struct _BLCast<BLCircle   > : public BLStructureConverterImpl<BLCircle   , Circle2d   > { using type = tbtype; };
template <> struct _BLCast<BLEllipse  > : public BLStructureConverterImpl<BLEllipse  , Ellipse2d  > { using type = tbtype; };
template <> struct _BLCast<BLLine     > : public BLStructureConverterImpl<BLLine     , Line2d     > { using type = tbtype; };
template <> struct _BLCast<BLRoundRect> : public BLStructureConverterImpl<BLRoundRect, RoundRect2d> { using type = tbtype; };
template <> struct _BLCast<BLTriangle > : public BLStructureConverterImpl<BLTriangle , Triangle2d > { using type = tbtype; };

template <> struct _BLCast<Point2d    > : public BLStructureConverterImpl<BLPoint    , Point2d    > { using type = bltype; };
template <> struct _BLCast<Point2i    > : public BLStructureConverterImpl<BLPointI   , Point2i    > { using type = bltype; };
template <> struct _BLCast<Rect2d     > : public BLStructureConverterImpl<BLRect     , Rect2d     > { using type = bltype; };
template <> struct _BLCast<Rect2i     > : public BLStructureConverterImpl<BLRectI    , Rect2i     > { using type = bltype; };
template <> struct _BLCast<Size2d     > : public BLStructureConverterImpl<BLSize     , Size2d     > { using type = bltype; };
template <> struct _BLCast<Size2i     > : public BLStructureConverterImpl<BLSizeI    , Size2i     > { using type = bltype; };
template <> struct _BLCast<Arc2d      > : public BLStructureConverterImpl<BLArc      , Arc2d      > { using type = bltype; };
template <> struct _BLCast<Circle2d   > : public BLStructureConverterImpl<BLCircle   , Circle2d   > { using type = bltype; };
template <> struct _BLCast<Ellipse2d  > : public BLStructureConverterImpl<BLEllipse  , Ellipse2d  > { using type = bltype; };
template <> struct _BLCast<Line2d     > : public BLStructureConverterImpl<BLLine     , Line2d     > { using type = bltype; };
template <> struct _BLCast<RoundRect2d> : public BLStructureConverterImpl<BLRoundRect, RoundRect2d> { using type = bltype; };
template <> struct _BLCast<Triangle2d > : public BLStructureConverterImpl<BLTriangle , Triangle2d > { using type = bltype; };
// clang-format on

template <typename T>
static auto _cast(T p) TBAG_NOEXCEPT
{
    return _BLCast<typename libtbag::remove_cpr<T>::type>::cast(p);
}

static uint32_t __get_channels_to_bl_format(int channels) TBAG_NOEXCEPT
{
    switch (channels) {
    case 3: return BL_FORMAT_XRGB32;
    case 4: return BL_FORMAT_PRGB32;
    default: return BL_FORMAT_NONE;
    }
}

static uint32_t __get_extend_mode_to_bl_extend_mode(ExtendMode mode) TBAG_NOEXCEPT
{
    // clang-format off
    switch (mode) {
    case ExtendMode::EM_PAD:                 return BL_EXTEND_MODE_PAD;
    case ExtendMode::EM_REPEAT:              return BL_EXTEND_MODE_REPEAT;
    case ExtendMode::EM_REFLECT:             return BL_EXTEND_MODE_REFLECT;
    case ExtendMode::EM_PAD_X_PAD_Y:         return BL_EXTEND_MODE_PAD_X_PAD_Y;
    case ExtendMode::EM_REPEAT_X_REPEAT_Y:   return BL_EXTEND_MODE_REPEAT_X_REPEAT_Y;
    case ExtendMode::EM_REFLECT_X_REFLECT_Y: return BL_EXTEND_MODE_REFLECT_X_REFLECT_Y;
    case ExtendMode::EM_PAD_X_REPEAT_Y:      return BL_EXTEND_MODE_PAD_X_REPEAT_Y;
    case ExtendMode::EM_PAD_X_REFLECT_Y:     return BL_EXTEND_MODE_PAD_X_REFLECT_Y;
    case ExtendMode::EM_REPEAT_X_PAD_Y:      return BL_EXTEND_MODE_REPEAT_X_PAD_Y;
    case ExtendMode::EM_REPEAT_X_REFLECT_Y:  return BL_EXTEND_MODE_REPEAT_X_REFLECT_Y;
    case ExtendMode::EM_REFLECT_X_PAD_Y:     return BL_EXTEND_MODE_REFLECT_X_PAD_Y;
    case ExtendMode::EM_REFLECT_X_REPEAT_Y:  return BL_EXTEND_MODE_REFLECT_X_REPEAT_Y;
    default:
        return BL_EXTEND_MODE_REPEAT;
    }
    // clang-format on
}

static ExtendMode __get_bl_extend_mode_to_extend_mode(uint32_t mode) TBAG_NOEXCEPT
{
    static_assert(BL_EXTEND_MODE_PAD == BL_EXTEND_MODE_PAD_X_PAD_Y, "");
    static_assert(BL_EXTEND_MODE_REPEAT == BL_EXTEND_MODE_REPEAT_X_REPEAT_Y, "");
    static_assert(BL_EXTEND_MODE_REFLECT == BL_EXTEND_MODE_REFLECT_X_REFLECT_Y, "");

    // clang-format off
    switch (mode) {
    case BL_EXTEND_MODE_PAD:                 return ExtendMode::EM_PAD;
    case BL_EXTEND_MODE_REPEAT:              return ExtendMode::EM_REPEAT;
    case BL_EXTEND_MODE_REFLECT:             return ExtendMode::EM_REFLECT;
    case BL_EXTEND_MODE_PAD_X_REPEAT_Y:      return ExtendMode::EM_PAD_X_REPEAT_Y;
    case BL_EXTEND_MODE_PAD_X_REFLECT_Y:     return ExtendMode::EM_PAD_X_REFLECT_Y;
    case BL_EXTEND_MODE_REPEAT_X_PAD_Y:      return ExtendMode::EM_REPEAT_X_PAD_Y;
    case BL_EXTEND_MODE_REPEAT_X_REFLECT_Y:  return ExtendMode::EM_REPEAT_X_REFLECT_Y;
    case BL_EXTEND_MODE_REFLECT_X_PAD_Y:     return ExtendMode::EM_REFLECT_X_PAD_Y;
    case BL_EXTEND_MODE_REFLECT_X_REPEAT_Y:  return ExtendMode::EM_REFLECT_X_REPEAT_Y;
    default:
        return ExtendMode::EM_REPEAT;
    }
    // clang-format on
}

static uint32_t __get_blend_mode_to_bl_comp_op(BlendMode mode) TBAG_NOEXCEPT
{
    // clang-format off
    switch (mode) {
    case BlendMode::BM_SRC_OVER    : return BL_COMP_OP_SRC_OVER;
    case BlendMode::BM_SRC_COPY    : return BL_COMP_OP_SRC_COPY;
    case BlendMode::BM_SRC_IN      : return BL_COMP_OP_SRC_IN;
    case BlendMode::BM_SRC_OUT     : return BL_COMP_OP_SRC_OUT;
    case BlendMode::BM_SRC_ATOP    : return BL_COMP_OP_SRC_ATOP;
    case BlendMode::BM_DST_OVER    : return BL_COMP_OP_DST_OVER;
    case BlendMode::BM_DST_COPY    : return BL_COMP_OP_DST_COPY;
    case BlendMode::BM_DST_IN      : return BL_COMP_OP_DST_IN;
    case BlendMode::BM_DST_OUT     : return BL_COMP_OP_DST_OUT;
    case BlendMode::BM_DST_ATOP    : return BL_COMP_OP_DST_ATOP;
    case BlendMode::BM_XOR         : return BL_COMP_OP_XOR;
    case BlendMode::BM_CLEAR       : return BL_COMP_OP_CLEAR;
    case BlendMode::BM_PLUS        : return BL_COMP_OP_PLUS;
    case BlendMode::BM_MINUS       : return BL_COMP_OP_MINUS;
    case BlendMode::BM_MULTIPLY    : return BL_COMP_OP_MULTIPLY;
    case BlendMode::BM_SCREEN      : return BL_COMP_OP_SCREEN;
    case BlendMode::BM_OVERLAY     : return BL_COMP_OP_OVERLAY;
    case BlendMode::BM_DARKEN      : return BL_COMP_OP_DARKEN;
    case BlendMode::BM_LIGHTEN     : return BL_COMP_OP_LIGHTEN;
    case BlendMode::BM_COLOR_DODGE : return BL_COMP_OP_COLOR_DODGE;
    case BlendMode::BM_COLOR_BURN  : return BL_COMP_OP_COLOR_BURN;
    case BlendMode::BM_LINEAR_BURN : return BL_COMP_OP_LINEAR_BURN;
    case BlendMode::BM_LINEAR_LIGHT: return BL_COMP_OP_LINEAR_LIGHT;
    case BlendMode::BM_PIN_LIGHT   : return BL_COMP_OP_PIN_LIGHT;
    case BlendMode::BM_HARD_LIGHT  : return BL_COMP_OP_HARD_LIGHT;
    case BlendMode::BM_SOFT_LIGHT  : return BL_COMP_OP_SOFT_LIGHT;
    case BlendMode::BM_DIFFERENCE  : return BL_COMP_OP_DIFFERENCE;
    case BlendMode::BM_EXCLUSION   : return BL_COMP_OP_EXCLUSION;
    default:
        return 0;
    }
    // clang-format on
}

static BlendMode __get_bl_comp_op_to_blend_mode(uint32_t mode) TBAG_NOEXCEPT
{
    // clang-format off
    switch (mode) {
    case BL_COMP_OP_SRC_OVER    : return BlendMode::BM_SRC_OVER;
    case BL_COMP_OP_SRC_COPY    : return BlendMode::BM_SRC_COPY;
    case BL_COMP_OP_SRC_IN      : return BlendMode::BM_SRC_IN;
    case BL_COMP_OP_SRC_OUT     : return BlendMode::BM_SRC_OUT;
    case BL_COMP_OP_SRC_ATOP    : return BlendMode::BM_SRC_ATOP;
    case BL_COMP_OP_DST_OVER    : return BlendMode::BM_DST_OVER;
    case BL_COMP_OP_DST_COPY    : return BlendMode::BM_DST_COPY;
    case BL_COMP_OP_DST_IN      : return BlendMode::BM_DST_IN;
    case BL_COMP_OP_DST_OUT     : return BlendMode::BM_DST_OUT;
    case BL_COMP_OP_DST_ATOP    : return BlendMode::BM_DST_ATOP;
    case BL_COMP_OP_XOR         : return BlendMode::BM_XOR;
    case BL_COMP_OP_CLEAR       : return BlendMode::BM_CLEAR;
    case BL_COMP_OP_PLUS        : return BlendMode::BM_PLUS;
    case BL_COMP_OP_MINUS       : return BlendMode::BM_MINUS;
    case BL_COMP_OP_MULTIPLY    : return BlendMode::BM_MULTIPLY;
    case BL_COMP_OP_SCREEN      : return BlendMode::BM_SCREEN;
    case BL_COMP_OP_OVERLAY     : return BlendMode::BM_OVERLAY;
    case BL_COMP_OP_DARKEN      : return BlendMode::BM_DARKEN;
    case BL_COMP_OP_LIGHTEN     : return BlendMode::BM_LIGHTEN;
    case BL_COMP_OP_COLOR_DODGE : return BlendMode::BM_COLOR_DODGE;
    case BL_COMP_OP_COLOR_BURN  : return BlendMode::BM_COLOR_BURN;
    case BL_COMP_OP_LINEAR_BURN : return BlendMode::BM_LINEAR_BURN;
    case BL_COMP_OP_LINEAR_LIGHT: return BlendMode::BM_LINEAR_LIGHT;
    case BL_COMP_OP_PIN_LIGHT   : return BlendMode::BM_PIN_LIGHT;
    case BL_COMP_OP_HARD_LIGHT  : return BlendMode::BM_HARD_LIGHT;
    case BL_COMP_OP_SOFT_LIGHT  : return BlendMode::BM_SOFT_LIGHT;
    case BL_COMP_OP_DIFFERENCE  : return BlendMode::BM_DIFFERENCE;
    case BL_COMP_OP_EXCLUSION   : return BlendMode::BM_EXCLUSION;
    default:
        return BlendMode::BM_SRC_OVER;
    }
    // clang-format on
}

static BLGeometryDirection __get_geo_dir_to_bl_geo_dir(GeometryDirection dir) TBAG_NOEXCEPT
{
    switch (dir) {
    case GeometryDirection::GD_NONE:
        return BL_GEOMETRY_DIRECTION_NONE;
    case GeometryDirection::GD_CW:
        return BL_GEOMETRY_DIRECTION_CW;
    case GeometryDirection::GD_CCW:
        return BL_GEOMETRY_DIRECTION_CCW;
    default:
        return BL_GEOMETRY_DIRECTION_NONE;
    }
}

Err convertBLResultToErr(uint32_t code) TBAG_NOEXCEPT
{
    // clang-format off
    switch (code) {
    case BL_SUCCESS:                            return E_SUCCESS;
    case BL_ERROR_OUT_OF_MEMORY:                return E_ENOMEM;
    case BL_ERROR_INVALID_VALUE:                return E_EINVAL;
    case BL_ERROR_INVALID_STATE:                return E_EFAULT;
    case BL_ERROR_INVALID_HANDLE:               return E_EBADF;
    case BL_ERROR_VALUE_TOO_LARGE:              return E_EOVERFLOW;
    case BL_ERROR_NOT_INITIALIZED:              return E_NREADY;
    case BL_ERROR_NOT_IMPLEMENTED:              return E_ENOSYS;
    case BL_ERROR_NOT_PERMITTED:                return E_EPERM;
    case BL_ERROR_IO:                           return E_EIO;
    case BL_ERROR_BUSY:                         return E_EBUSY;
    case BL_ERROR_INTERRUPTED:                  return E_EINTR;
    case BL_ERROR_TRY_AGAIN:                    return E_EAGAIN;
    case BL_ERROR_TIMED_OUT:                    return E_ETIMEDOUT;
    case BL_ERROR_BROKEN_PIPE:                  return E_EPIPE;
    case BL_ERROR_INVALID_SEEK:                 return E_ESPIPE;
    case BL_ERROR_SYMLINK_LOOP:                 return E_ELOOP;
    case BL_ERROR_FILE_TOO_LARGE:               return E_EFBIG;
    case BL_ERROR_ALREADY_EXISTS:               return E_EEXIST;
    case BL_ERROR_ACCESS_DENIED:                return E_EACCES;
    case BL_ERROR_MEDIA_CHANGED:                return E_MEDIA_CHANGED;
    case BL_ERROR_READ_ONLY_FS:                 return E_EROFS;
    case BL_ERROR_NO_DEVICE:                    return E_ENXIO;
    case BL_ERROR_NO_ENTRY:                     return E_ENOENT;
    case BL_ERROR_NO_MEDIA:                     return E_NO_MEDIA;
    case BL_ERROR_NO_MORE_DATA:                 return E_ENODATA;
    case BL_ERROR_NO_MORE_FILES:                return E_NO_MORE_FILES;
    case BL_ERROR_NO_SPACE_LEFT:                return E_ENOSPC;
    case BL_ERROR_NOT_EMPTY:                    return E_ENOTEMPTY;
    case BL_ERROR_NOT_FILE:                     return E_EISDIR;
    case BL_ERROR_NOT_DIRECTORY:                return E_ENOTDIR;
    case BL_ERROR_NOT_SAME_DEVICE:              return E_EXDEV;
    case BL_ERROR_NOT_BLOCK_DEVICE:             return E_NOT_BLOCK_DEVICE;
    case BL_ERROR_INVALID_FILE_NAME:            return E_INVALID_FILE_NAME;
    case BL_ERROR_FILE_NAME_TOO_LONG:           return E_ENAMETOOLONG;
    case BL_ERROR_TOO_MANY_OPEN_FILES:          return E_EMFILE;
    case BL_ERROR_TOO_MANY_OPEN_FILES_BY_OS:    return E_ENFILE;
    case BL_ERROR_TOO_MANY_LINKS:               return E_EMLINK;
    case BL_ERROR_TOO_MANY_THREADS:             return E_EAGAIN;
    case BL_ERROR_FILE_EMPTY:                   return E_FILE_EMPTY;
    case BL_ERROR_OPEN_FAILED:                  return E_OPEN_FAILED;
    case BL_ERROR_NOT_ROOT_DEVICE:              return E_NOT_ROOT_DEVICE;
    case BL_ERROR_UNKNOWN_SYSTEM_ERROR:         return E_UNKNOWN_SYSTEM_ERROR;
    case BL_ERROR_INVALID_ALIGNMENT:            return E_ILLARGS;
    case BL_ERROR_INVALID_SIGNATURE:            return E_INVALID_SIGNATURE;
    case BL_ERROR_INVALID_DATA:                 return E_INVALID_DATA;
    case BL_ERROR_INVALID_STRING:               return E_INVALID_STRING;
    case BL_ERROR_DATA_TRUNCATED:               return E_DATA_TRUNCATED;
    case BL_ERROR_DATA_TOO_LARGE:               return E_DATA_TOO_LARGE;
    case BL_ERROR_DECOMPRESSION_FAILED:         return E_DECOMPRESSION_FAILED;
    case BL_ERROR_INVALID_GEOMETRY:             return E_INVALID_GEOMETRY;
    case BL_ERROR_NO_MATCHING_VERTEX:           return E_NO_MATCHING_VERTEX;
    case BL_ERROR_NO_MATCHING_COOKIE:           return E_NO_MATCHING_COOKIE;
    case BL_ERROR_NO_STATES_TO_RESTORE:         return E_NO_STATES_TO_RESTORE;
    case BL_ERROR_IMAGE_TOO_LARGE:              return E_IMAGE_TOO_LARGE;
    case BL_ERROR_IMAGE_NO_MATCHING_CODEC:      return E_IMAGE_NO_MATCHING_CODEC;
    case BL_ERROR_IMAGE_UNKNOWN_FILE_FORMAT:    return E_IMAGE_UNKNOWN_FILE_FORMAT;
    case BL_ERROR_IMAGE_DECODER_NOT_PROVIDED:   return E_IMAGE_DECODER_NOT_PROVIDED;
    case BL_ERROR_IMAGE_ENCODER_NOT_PROVIDED:   return E_IMAGE_ENCODER_NOT_PROVIDED;
    case BL_ERROR_PNG_MULTIPLE_IHDR:            return E_PNG_MULTIPLE_IHDR;
    case BL_ERROR_PNG_INVALID_IDAT:             return E_PNG_INVALID_IDAT;
    case BL_ERROR_PNG_INVALID_IEND:             return E_PNG_INVALID_IEND;
    case BL_ERROR_PNG_INVALID_PLTE:             return E_PNG_INVALID_PLTE;
    case BL_ERROR_PNG_INVALID_TRNS:             return E_PNG_INVALID_TRNS;
    case BL_ERROR_PNG_INVALID_FILTER:           return E_PNG_INVALID_FILTER;
    case BL_ERROR_JPEG_UNSUPPORTED_FEATURE:     return E_JPEG_UNSUPPORTED_FEATURE;
    case BL_ERROR_JPEG_INVALID_SOS:             return E_JPEG_INVALID_SOS;
    case BL_ERROR_JPEG_INVALID_SOF:             return E_JPEG_INVALID_SOF;
    case BL_ERROR_JPEG_MULTIPLE_SOF:            return E_JPEG_MULTIPLE_SOF;
    case BL_ERROR_JPEG_UNSUPPORTED_SOF:         return E_JPEG_UNSUPPORTED_SOF;
    case BL_ERROR_FONT_NO_CHARACTER_MAPPING:    return E_FONT_NO_CHARACTER_MAPPING;
    case BL_ERROR_FONT_MISSING_IMPORTANT_TABLE: return E_FONT_MISSING_IMPORTANT_TABLE;
    case BL_ERROR_FONT_FEATURE_NOT_AVAILABLE:   return E_FONT_FEATURE_NOT_AVAILABLE;
    case BL_ERROR_FONT_CFF_INVALID_DATA:        return E_FONT_CFF_INVALID_DATA;
    case BL_ERROR_FONT_PROGRAM_TERMINATED:      return E_FONT_PROGRAM_TERMINATED;
    case BL_ERROR_INVALID_GLYPH:                return E_INVALID_GLYPH;
    default:                                    return E_UNKNOWN;
    }
    // clang-format on
}

static Err __create_rgba_image(Box const & box, BLImage & dest)
{
    auto const height = box.dim(0);
    auto const width = box.dim(1);
    auto const channels = box.dim(2);
    if (height <= 0 || width <= 0 || channels != 4) {
        return E_ILLARGS;
    }
    auto const format = __get_channels_to_bl_format(channels);
    assert(format == BL_FORMAT_PRGB32);
    auto const stride = box.getStrideByte(0);
    assert(stride >= 4 && stride % 4 == 0);
    return convertBLResultToErr(dest.createFromData(width, height, format, (void*)box.data(), stride));
}

static Err __create_bl_image(BLImage const & src, Box & dest)
{
    BLImageData data = {};
    auto const obtain_code = convertBLResultToErr(src.getData(&data));
    if (isFailure(obtain_code)) {
        return obtain_code;
    }

    auto const width = src.width();
    auto const height = src.height();
    auto const format = src.format();

    int channels;
    switch (format) {
    case BL_FORMAT_XRGB32:
        channels = 3;
        break;
    case BL_FORMAT_PRGB32:
        channels = 4;
        break;
    default:
        return E_ENOSYS;
    }

    auto const resize_code = dest.resize<std::uint8_t>(height, width, channels);
    if (isFailure(resize_code)) {
        return resize_code;
    }

    if (channels == 3) {
        auto const * source = static_cast<std::uint8_t const *>(data.pixelData);
        auto * destination = dest.data<std::uint8_t>();
        auto const loop_size = height*width;
        for (auto i = 0; i < loop_size; ++i) {
            destination[0] = source[0];
            destination[1] = source[1];
            destination[2] = source[2];
            // destination[-] = source[3]; // Alpha value!
            destination += 3;
            source += 4;
        }
    } else if (channels == 4) {
        memcpy(dest.data(), data.pixelData, (height*width*channels));
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_INACCESSIBLE_BLOCK;
    }
    return E_SUCCESS;
}

static std::vector<BLGradientStop> __get_gradient_stop_arr(GradientStop const * stops, size_t n)
{
    std::vector<BLGradientStop> result(n);
    for (auto i = 0; i < n; ++i) {
        result[i] = _cvt(stops[i]);
    }
    return result;
}

/**
 * Canvas::Impl class implementation.
 *
 * @author zer0
 * @date   2020-01-11
 */
struct Canvas::Impl : private Noncopyable
{
    int width;
    int height;
    int channels;

    BLImage image;
    BLContext base;

    Impl(int w, int h, int c)
            : width(w), height(h), channels(c),
              image(w, h, __get_channels_to_bl_format(c)),
              base(image)
    { /* EMPTY. */ }
    ~Impl()
    { /* EMPTY. */ }

    template <typename PointT>
    Err blit_image(PointT p, Canvas::Box const & src)
    {
        BLImage temp;
        auto const code = __create_rgba_image(src, temp);
        if (isFailure(code)) {
            return code;
        }
        return convertBLResultToErr(base.blitImage(_cvt(p), temp));
    }

    template <typename PointT, typename RectT>
    Err blit_image(PointT p, Canvas::Box const & src, RectT r)
    {
        BLImage temp;
        auto const code = __create_rgba_image(src, temp);
        if (isFailure(code)) {
            return code;
        }
        return convertBLResultToErr(base.blitImage(_cvt(p), temp, _cvt(r)));
    }
};

// clang-format off
struct Canvas::ImageImpl       : private Noncopyable { BLImage       base; };
struct Canvas::PatternImpl     : private Noncopyable { BLPattern     base; };
struct Canvas::GradientImpl    : private Noncopyable { BLGradient    base; };
struct Canvas::PathImpl        : private Noncopyable { BLPath        base; };
struct Canvas::FontLoaderImpl  : private Noncopyable { BLFontLoader  base; };
struct Canvas::FontFaceImpl    : private Noncopyable { BLFontFace    base; };
struct Canvas::GlyphBufferImpl : private Noncopyable { BLGlyphBuffer base; };
struct Canvas::FontImpl        : private Noncopyable { BLFont        base; };
// clang-format on

#ifndef _call_blend2d_method
#define _call_blend2d_method(func, ...) \
    ((_impl) ? (convertBLResultToErr(_impl->base.func(__VA_ARGS__))) : (E_NREADY))
#endif

#ifndef _call_blend2d_getter
#define _call_blend2d_getter(func, def) \
    ((_impl) ? (_impl->base.func()) : (def))
#endif

// ----------------------------
// Canvas::Image implementation
// ----------------------------

Canvas::Image::Image() : _impl(std::make_shared<ImageImpl>())
{
    assert(_impl);
}

Err Canvas::Image::create(int w, int h, uint32_t format) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, w, h, format);
}

int Canvas::Image::width() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(width, 0);
}

int Canvas::Image::height() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(height, 0);
}

uint32_t Canvas::Image::format() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(format, 0);
}

Err Canvas::Image::readFromFile(char const * file_name) TBAG_NOEXCEPT
{
    return _call_blend2d_method(readFromFile, file_name);
}

Err Canvas::Image::readFromData(void const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(readFromData, data, size);
}

Err Canvas::Image::readFromBox(Box const & box)
{
    return _impl ? __create_rgba_image(box, _impl->base) : E_NREADY;
}

Err Canvas::Image::writeToBox(Box & box) const
{
    return _impl ? __create_bl_image(_impl->base, box) : E_NREADY;
}

// ------------------------------
// Canvas::Pattern implementation
// ------------------------------

Canvas::Pattern::Pattern() : _impl(std::make_shared<PatternImpl>())
{
    assert(_impl);
}

Err Canvas::Pattern::create(Image const & image, ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, image._impl->base, __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Pattern::create(Image const & image, ExtendMode extend_mode, Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, image._impl->base,
                                __get_extend_mode_to_bl_extend_mode(extend_mode), _cvt(m));
}

Err Canvas::Pattern::create(Image const & image, Rect2i const & area, ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, image._impl->base, _cast(area),
                                __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Pattern::create(Image const & image, Rect2i const & area, ExtendMode extend_mode, Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, image._impl->base, _cast(area),
                                __get_extend_mode_to_bl_extend_mode(extend_mode), _cvt(m));
}

Err Canvas::Pattern::setImage(Image const & image) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setImage, image._impl->base);
}

Err Canvas::Pattern::setImage(Image const & image, Rect2i const & area) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setImage, image._impl->base, _cast(area));
}

Err Canvas::Pattern::resetImage() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetImage);
}

Rect2i Canvas::Pattern::getArea() const TBAG_NOEXCEPT
{
    return _cast(_impl->base.area());
}

Err Canvas::Pattern::setArea(Rect2i const & area) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setArea, _cast(area));
}

Err Canvas::Pattern::resetArea() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetArea);
}

ExtendMode Canvas::Pattern::getExtendMode() const TBAG_NOEXCEPT
{
    return __get_bl_extend_mode_to_extend_mode(_impl->base.extendMode());
}

Err Canvas::Pattern::setExtendMode(ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setExtendMode, __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Pattern::resetExtendMode() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetExtendMode);
}

bool Canvas::Pattern::hasMatrix() const TBAG_NOEXCEPT
{
    return _impl->base.hasMatrix();
}

uint32_t Canvas::Pattern::getMatrixType() const TBAG_NOEXCEPT
{
    return _impl->base.matrixType();
}

Matrix Canvas::Pattern::getMatrix() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.matrix());
}

Err Canvas::Pattern::setMatrix(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setMatrix, _cvt(m));
}

Err Canvas::Pattern::resetMatrix() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetMatrix);
}

Err Canvas::Pattern::translate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, x, y);
}

Err Canvas::Pattern::translate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::Pattern::translate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::Pattern::scale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, xy);
}

Err Canvas::Pattern::scale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, x, y);
}

Err Canvas::Pattern::scale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::Pattern::scale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::Pattern::skew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, x, y);
}

Err Canvas::Pattern::skew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, _cast(p));
}

Err Canvas::Pattern::rotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle);
}

Err Canvas::Pattern::rotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, x, y);
}

Err Canvas::Pattern::rotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::Pattern::rotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::Pattern::transform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, _cvt(m));
}

Err Canvas::Pattern::postTranslate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, x, y);
}

Err Canvas::Pattern::postTranslate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::Pattern::postTranslate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::Pattern::postScale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, xy);
}

Err Canvas::Pattern::postScale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, x, y);
}

Err Canvas::Pattern::postScale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::Pattern::postScale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::Pattern::postSkew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, x, y);
}

Err Canvas::Pattern::postSkew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, _cast(p));
}

Err Canvas::Pattern::postRotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle);
}

Err Canvas::Pattern::postRotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, x, y);
}

Err Canvas::Pattern::postRotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::Pattern::postRotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::Pattern::postTransform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTransform, _cvt(m));
}

// -------------------------------
// Canvas::Gradient implementation
// -------------------------------

Canvas::Gradient::Gradient() : _impl(std::make_shared<GradientImpl>())
{
    assert(_impl);
}

Err Canvas::Gradient::create(LinearGradientValues const & values, ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, _cvt(values), __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Gradient::create(RadialGradientValues const & values, ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, _cvt(values), __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Gradient::create(ConicalGradientValues const & values, ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(create, _cvt(values), __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Gradient::create(LinearGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n);
}

Err Canvas::Gradient::create(RadialGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n);
}

Err Canvas::Gradient::create(ConicalGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n);
}

Err Canvas::Gradient::create(LinearGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n, _cvt(m));
}

Err Canvas::Gradient::create(RadialGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n, _cvt(m));
}

Err Canvas::Gradient::create(ConicalGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    auto const mode = __get_extend_mode_to_bl_extend_mode(extend_mode);
    return _call_blend2d_method(create, _cvt(values), mode, temp.data(), n, _cvt(m));
}

uint32_t Canvas::Gradient::getType() const TBAG_NOEXCEPT
{
    return _impl->base.type();
}

Err Canvas::Gradient::setType(uint32_t type) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setType, type);
}

ExtendMode Canvas::Gradient::getExtendMode() const TBAG_NOEXCEPT
{
    return __get_bl_extend_mode_to_extend_mode(_impl->base.extendMode());
}

Err Canvas::Gradient::setExtendMode(ExtendMode extend_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setExtendMode, __get_extend_mode_to_bl_extend_mode(extend_mode));
}

Err Canvas::Gradient::resetExtendMode() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetExtendMode);
}

double Canvas::Gradient::getValue(size_t index) const TBAG_NOEXCEPT
{
    return _impl->base.value(index);
}

LinearGradientValues Canvas::Gradient::getLinear() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.linear());
}

RadialGradientValues Canvas::Gradient::getRadial() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.radial());
}

ConicalGradientValues Canvas::Gradient::getConical() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.conical());
}

Err Canvas::Gradient::setValue(size_t index, double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setValue, index, value);
}

Err Canvas::Gradient::setValues(size_t index, double const * values, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setValues, index, values, n);
}

Err Canvas::Gradient::setValues(LinearGradientValues const & values) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setValues, _cvt(values));
}

Err Canvas::Gradient::setValues(RadialGradientValues const & values) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setValues, _cvt(values));
}

Err Canvas::Gradient::setValues(ConicalGradientValues const & values) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setValues, _cvt(values));
}

double Canvas::Gradient::x0() const TBAG_NOEXCEPT
{
    return _impl->base.x0();
}

double Canvas::Gradient::y0() const TBAG_NOEXCEPT
{
    return _impl->base.y0();
}

double Canvas::Gradient::x1() const TBAG_NOEXCEPT
{
    return _impl->base.x1();
}

double Canvas::Gradient::y1() const TBAG_NOEXCEPT
{
    return _impl->base.y1();
}

double Canvas::Gradient::r0() const TBAG_NOEXCEPT
{
    return _impl->base.r0();
}

double Canvas::Gradient::angle() const TBAG_NOEXCEPT
{
    return _impl->base.angle();
}

Err Canvas::Gradient::setX0(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setX0, value);
}

Err Canvas::Gradient::setY0(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setY0, value);
}

Err Canvas::Gradient::setX1(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setX1, value);
}

Err Canvas::Gradient::setY1(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setY1, value);
}

Err Canvas::Gradient::setR0(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setR0, value);
}

Err Canvas::Gradient::setAngle(double value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setAngle, value);
}

bool Canvas::Gradient::empty() const TBAG_NOEXCEPT
{
    return _impl->base.empty();
}

size_t Canvas::Gradient::size() const TBAG_NOEXCEPT
{
    return _impl->base.size();
}

size_t Canvas::Gradient::capacity() const TBAG_NOEXCEPT
{
    return _impl->base.capacity();
}

Err Canvas::Gradient::reserve(size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(reserve, n);
}

Err Canvas::Gradient::shrink() TBAG_NOEXCEPT
{
    return _call_blend2d_method(shrink);
}

std::vector<GradientStop> Canvas::Gradient::stops() const TBAG_NOEXCEPT
{
    auto const size = _impl->base.size();
    std::vector<GradientStop> result(size);
    for (auto i = 0; i < size; ++i) {
        result[i] = getStop(i);
    }
    return result;
}

GradientStop Canvas::Gradient::getStop(size_t i) const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.stopAt(i));
}

Err Canvas::Gradient::resetStops() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetStops);
}

Err Canvas::Gradient::assignStops(GradientStop const * stops, size_t n) TBAG_NOEXCEPT
{
    auto const temp = __get_gradient_stop_arr(stops, n);
    return _call_blend2d_method(assignStops, temp.data(), n);
}

Err Canvas::Gradient::addStop(double offset, Rgb32 const & rgba32) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addStop, offset, _cvt(rgba32));
}

Err Canvas::Gradient::removeStop(size_t index) TBAG_NOEXCEPT
{
    return _call_blend2d_method(removeStop, index);
}

Err Canvas::Gradient::removeStopByOffset(double offset, bool all) TBAG_NOEXCEPT
{
    return _call_blend2d_method(removeStopByOffset, offset, all);
}

Err Canvas::Gradient::removeStops(Range const & range) TBAG_NOEXCEPT
{
    return _call_blend2d_method(removeStops, _cvt(range));
}

Err Canvas::Gradient::removeStopsByOffset(double offset_min, double offset_max) TBAG_NOEXCEPT
{
    return _call_blend2d_method(removeStopsByOffset, offset_min, offset_max);
}

Err Canvas::Gradient::replaceStop(size_t index, double offset, Rgb32 const & rgba32) TBAG_NOEXCEPT
{
    return _call_blend2d_method(replaceStop, index, offset, _cvt(rgba32));
}

size_t Canvas::Gradient::getIndexOfStop(double offset) const TBAG_NOEXCEPT
{
    return _impl->base.indexOfStop(offset);
}

bool Canvas::Gradient::hasMatrix() const TBAG_NOEXCEPT
{
    return _impl->base.hasMatrix();
}

uint32_t Canvas::Gradient::getMatrixType() const TBAG_NOEXCEPT
{
    return _impl->base.matrixType();
}

Matrix Canvas::Gradient::getMatrix() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.matrix());
}

Err Canvas::Gradient::setMatrix(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setMatrix, _cvt(m));
}

Err Canvas::Gradient::resetMatrix() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetMatrix);
}

Err Canvas::Gradient::translate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, x, y);
}

Err Canvas::Gradient::translate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::Gradient::translate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::Gradient::scale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, xy);
}

Err Canvas::Gradient::scale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, x, y);
}

Err Canvas::Gradient::scale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::Gradient::scale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::Gradient::skew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, x, y);
}

Err Canvas::Gradient::skew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, _cast(p));
}

Err Canvas::Gradient::rotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle);
}

Err Canvas::Gradient::rotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, x, y);
}

Err Canvas::Gradient::rotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::Gradient::rotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::Gradient::transform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, _cvt(m));
}

Err Canvas::Gradient::postTranslate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, x, y);
}

Err Canvas::Gradient::postTranslate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::Gradient::postTranslate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::Gradient::postScale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, xy);
}

Err Canvas::Gradient::postScale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, x, y);
}

Err Canvas::Gradient::postScale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::Gradient::postScale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::Gradient::postSkew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, x, y);
}

Err Canvas::Gradient::postSkew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, _cast(p));
}

Err Canvas::Gradient::postRotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle);
}

Err Canvas::Gradient::postRotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, x, y);
}

Err Canvas::Gradient::postRotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::Gradient::postRotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::Gradient::postTransform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTransform, _cvt(m));
}

// ---------------------------
// Canvas::Path implementation
// ---------------------------

Canvas::Path::Path() : _impl(std::make_shared<PathImpl>())
{
    assert(_impl);
}

size_t Canvas::Path::size() const TBAG_NOEXCEPT
{
    return _impl->base.size();
}

size_t Canvas::Path::capacity() const TBAG_NOEXCEPT
{
    return _impl->base.capacity();
}

Point2d const * Canvas::Path::vertexData() const TBAG_NOEXCEPT
{
    return _cast(_impl->base.vertexData());
}

Point2d const * Canvas::Path::vertexDataEnd() const TBAG_NOEXCEPT
{
    return _cast(_impl->base.vertexDataEnd());
}

uint8_t const * Canvas::Path::commandData() const TBAG_NOEXCEPT
{
    return _impl->base.commandData();
}

uint8_t const * Canvas::Path::commandDataEnd() const TBAG_NOEXCEPT
{
    return _impl->base.commandDataEnd();
}

Err Canvas::Path::clear() TBAG_NOEXCEPT
{
    return _call_blend2d_method(clear);
}

Err Canvas::Path::shrink() TBAG_NOEXCEPT
{
    return _call_blend2d_method(shrink);
}

Err Canvas::Path::reserve(size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(reserve, n);
}

Err Canvas::Path::modifyOp(uint32_t op, size_t n, uint8_t ** cmd_data_out, Point2d ** vtx_data_out) TBAG_NOEXCEPT
{
    return _call_blend2d_method(modifyOp, op, n, cmd_data_out, (BLPoint**)vtx_data_out);
}

Err Canvas::Path::setVertexAt(size_t index, uint32_t cmd, Point2d const & pt) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setVertexAt, index, cmd, _cast(pt));
}

Err Canvas::Path::setVertexAt(size_t index, uint32_t cmd, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setVertexAt, index, cmd, x, y);
}

Err Canvas::Path::moveTo(Point2d const & p0) TBAG_NOEXCEPT
{
    return _call_blend2d_method(moveTo, _cast(p0));
}

Err Canvas::Path::moveTo(double x0, double y0) TBAG_NOEXCEPT
{
    return _call_blend2d_method(moveTo, x0, y0);
}

Err Canvas::Path::lineTo(Point2d const & p1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(lineTo, _cast(p1));
}

Err Canvas::Path::lineTo(double x1, double y1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(lineTo, x1, y1);
}

Err Canvas::Path::polyTo(Point2d const * poly, size_t count) TBAG_NOEXCEPT
{
    return _call_blend2d_method(polyTo, _cast(poly), count);
}

Err Canvas::Path::quadTo(Point2d const & p1, Point2d const & p2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(quadTo, _cast(p1), _cast(p2));
}

Err Canvas::Path::quadTo(double x1, double y1, double x2, double y2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(quadTo, x1, y1, x2, y2);
}

Err Canvas::Path::cubicTo(Point2d const & p1, Point2d const & p2, Point2d const & p3) TBAG_NOEXCEPT
{
    return _call_blend2d_method(cubicTo, _cast(p1), _cast(p2), _cast(p3));
}

Err Canvas::Path::cubicTo(double x1, double y1, double x2, double y2, double x3, double y3) TBAG_NOEXCEPT
{
    return _call_blend2d_method(cubicTo, x1, y1, x2, y2, x3, y3);
}

Err Canvas::Path::smoothQuadTo(Point2d const & p2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(smoothQuadTo, _cast(p2));
}

Err Canvas::Path::smoothQuadTo(double x2, double y2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(smoothQuadTo, x2, y2);
}

Err Canvas::Path::smoothCubicTo(Point2d const & p2, Point2d const & p3) TBAG_NOEXCEPT
{
    return _call_blend2d_method(smoothCubicTo, _cast(p2), _cast(p3));
}

Err Canvas::Path::smoothCubicTo(double x2, double y2, double x3, double y3) TBAG_NOEXCEPT
{
    return _call_blend2d_method(smoothCubicTo, x2, y2, x3, y3);
}

Err Canvas::Path::arcTo(Point2d const & c, Point2d const & r, double start, double sweep, bool force_move_to) TBAG_NOEXCEPT
{
    return _call_blend2d_method(arcTo, _cast(c), _cast(r), start, sweep, force_move_to);
}

Err Canvas::Path::arcTo(double cx, double cy, double rx, double ry, double start, double sweep, bool force_move_to) TBAG_NOEXCEPT
{
    return _call_blend2d_method(arcTo, cx, cy, rx, ry, start, sweep, force_move_to);
}

Err Canvas::Path::arcQuadrantTo(Point2d const & p1, Point2d const & p2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(arcQuadrantTo, _cast(p1), _cast(p2));
}

Err Canvas::Path::arcQuadrantTo(double x1, double y1, double x2, double y2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(arcQuadrantTo, x1, y1, x2, y2);
}

Err Canvas::Path::ellipticArcTo(Point2d const & rp, double x_axis_rotation, bool large_arc_flag, bool sweep_flag, Point2d const & p1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(ellipticArcTo, _cast(rp), x_axis_rotation, large_arc_flag, sweep_flag, _cast(p1));
}

Err Canvas::Path::ellipticArcTo(double rx, double ry, double x_axis_rotation, bool large_arc_flag, bool sweep_flag, double x1, double y1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(ellipticArcTo, rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x1, y1);
}

Err Canvas::Path::close() TBAG_NOEXCEPT
{
    return _call_blend2d_method(close);
}

Err Canvas::Path::addBox(double x0, double y0, double x1, double y1, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addBox, x0, y0, x1, y1, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRect(double x, double y, double w, double h, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addBox, x, y, w, h, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addCircle(Circle2d const & circle, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addCircle, _cast(circle), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addCircle(Circle2d const & circle, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addCircle, _cast(circle), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addEllipse(Ellipse2d const & ellipse, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addEllipse, _cast(ellipse), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addEllipse(Ellipse2d const & ellipse, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addEllipse, _cast(ellipse), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRoundRect(RoundRect2d const & rr, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRoundRect, _cast(rr), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRoundRect(RoundRect2d const & rr, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRoundRect, _cast(rr), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addArc(Arc2d const & arc, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addArc, _cast(arc), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addArc(Arc2d const & arc, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addArc, _cast(arc), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addChord(Arc2d const & chord, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addChord, _cast(chord), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addChord(Arc2d const & chord, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addChord, _cast(chord), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPie(Arc2d const & pie, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPie, _cast(pie), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPie(Arc2d const & pie, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPie, _cast(pie), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addLine(Line2d const & line, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addLine, _cast(line), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addLine(Line2d const & line, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addLine, _cast(line), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addTriangle(Triangle2d const & triangle, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addTriangle, _cast(triangle), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addTriangle(Triangle2d const & triangle, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addTriangle, _cast(triangle), _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolyline(Point2i const * poly, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolyline, _cast(poly), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolyline(Point2i const * poly, size_t n, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolyline, _cast(poly), n, _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolyline(Point2d const * poly, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolyline, _cast(poly), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolyline(Point2d const * poly, size_t n, Matrix const & m, uint32_t dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolyline, _cast(poly), n, _cvt(m), dir);
}

Err Canvas::Path::addPolygon(Point2i const * poly, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolygon, _cast(poly), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolygon(Point2i const * poly, size_t n, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolygon, _cast(poly), n, _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolygon(Point2d const * poly, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolygon, _cast(poly), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPolygon(Point2d const * poly, size_t n, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPolygon, _cast(poly), n, _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRectArray(Rect2i const * data, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRectArray, _cast(data), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRectArray(Rect2i const * data, size_t n, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRectArray, _cast(data), n, _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRectArray(Rect2d const * data, size_t n, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRectArray, _cast(data), n, __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addRectArray(Rect2d const * data, size_t n, Matrix const & m, Gd dir) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addRectArray, _cast(data), n, _cvt(m), __get_geo_dir_to_bl_geo_dir(dir));
}

Err Canvas::Path::addPath(Path const & path) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base);
}

Err Canvas::Path::addPath(Path const & path, Range const & range) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base, _cvt(range));
}

Err Canvas::Path::addPath(Path const & path, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base, _cast(p));
}

Err Canvas::Path::addPath(Path const & path, Range const & range, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base, _cvt(range), _cast(p));
}

Err Canvas::Path::addPath(Path const & path, Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base, _cvt(m));
}

Err Canvas::Path::addPath(Path const & path, Range const & range, Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addPath, path._impl->base, _cvt(range), _cvt(m));
}

Err Canvas::Path::addReversedPath(Path const & path, uint32_t reverse_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addReversedPath, path._impl->base, reverse_mode);
}

Err Canvas::Path::addReversedPath(Path const & path, Range const & range, uint32_t reverse_mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(addReversedPath, path._impl->base, _cvt(range), reverse_mode);
}

Err Canvas::Path::addStrokedPath(Path const & path, Range const & range) TBAG_NOEXCEPT
{
    return E_ENOSYS;
}

Err Canvas::Path::addStrokedPath(Path const & path) TBAG_NOEXCEPT
{
    return E_ENOSYS;
}

Err Canvas::Path::translate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::Path::translate(Range const & range, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cvt(range), _cast(p));
}

Err Canvas::Path::transform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, _cvt(m));
}

Err Canvas::Path::transform(Range const & range, Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, _cvt(range), _cvt(m));
}

Err Canvas::Path::getControlBox(Rect2d * rect_out) const TBAG_NOEXCEPT
{
    BLBox temp = {};
    auto const code = _call_blend2d_method(getControlBox, &temp);
    if (isSuccess(code) && rect_out) {
        *rect_out = Rect2d(Point2d(temp.x0, temp.y0), Point2d(temp.x1, temp.y1));
    }
    return code;
}

Err Canvas::Path::getBoundingBox(Rect2d * rect_out) const TBAG_NOEXCEPT
{
    BLBox temp = {};
    auto const code = _call_blend2d_method(getBoundingBox, &temp);
    if (isSuccess(code) && rect_out) {
        *rect_out = Rect2d(Point2d(temp.x0, temp.y0), Point2d(temp.x1, temp.y1));
    }
    return code;
}

Err Canvas::Path::fitTo(Rect2d const & rect, uint32_t fit_flags) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fitTo, _cast(rect), fit_flags);
}

Err Canvas::Path::fitTo(Range const & range, Rect2d const & rect, uint32_t fit_flags) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fitTo, _cvt(range), _cast(rect), fit_flags);
}

Err Canvas::Path::getInfoFlags(uint32_t * flags_out) const TBAG_NOEXCEPT
{
    return _call_blend2d_method(getInfoFlags, flags_out);
}

Err Canvas::Path::getFigureRange(size_t index, Range * range_out) const TBAG_NOEXCEPT
{
    BLRange temp = {};
    auto const code _call_blend2d_method(getFigureRange, index, &temp);
    if (isSuccess(code) && range_out) {
        range_out->start = temp.start;
        range_out->end = temp.end;
    }
    return code;
}

Err Canvas::Path::getLastVertex(Point2d * vtx_out) TBAG_NOEXCEPT
{
    return _call_blend2d_method(getLastVertex, _cast(vtx_out));
}

Err Canvas::Path::getClosestVertex(Point2d const & p, double max_distance, size_t * index_out) const TBAG_NOEXCEPT
{
    return _call_blend2d_method(getClosestVertex, _cast(p), max_distance, index_out);
}

Err Canvas::Path::getClosestVertex(Point2d const & p, double max_distance, size_t * index_out, double * distance_out) const TBAG_NOEXCEPT
{
    return _call_blend2d_method(getClosestVertex, _cast(p), max_distance, index_out, distance_out);
}

uint32_t Canvas::Path::hitTest(Point2d const & p, uint32_t fill_rule) const TBAG_NOEXCEPT
{
    return _impl ? _impl->base.hitTest(_cast(p), fill_rule) : 0;
}

// ---------------------------------
// Canvas::FontLoader implementation
// ---------------------------------

Canvas::FontLoader::FontLoader() : _impl(std::make_shared<FontLoaderImpl>())
{
    assert(_impl);
}

Err Canvas::FontLoader::createFromFile(const char * file_name, uint32_t read_flags) TBAG_NOEXCEPT
{
    return _call_blend2d_method(createFromFile, file_name, read_flags);
}

Err Canvas::FontLoader::createFromData(const void * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(createFromData, data, size);
}

uint32_t Canvas::FontLoader::getFaceType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceType, 0);
}

uint32_t Canvas::FontLoader::getFaceCount() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceCount, 0);
}

uint32_t Canvas::FontLoader::getLoaderFlags() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(loaderFlags, 0);
}

// -------------------------------
// Canvas::FontFace implementation
// -------------------------------

Canvas::FontFace::FontFace() : _impl(std::make_shared<FontFaceImpl>())
{
    assert(_impl);
}

Err Canvas::FontFace::createFromFile(char const * file_name, uint32_t read_flags) TBAG_NOEXCEPT
{
    return _call_blend2d_method(createFromFile, file_name, read_flags);
}

Err Canvas::FontFace::createFromLoader(FontLoader const & loader, uint32_t face_index) TBAG_NOEXCEPT
{
    return _call_blend2d_method(createFromLoader, loader._impl->base, face_index);
}

uint32_t Canvas::FontFace::getWeight() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(weight, 0);
}

uint32_t Canvas::FontFace::getStretch() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(stretch, 0);
}

uint32_t Canvas::FontFace::getStyle() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(style, 0);
}

uint32_t Canvas::FontFace::getFaceType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceType, 0);
}

uint32_t Canvas::FontFace::getOutlineType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(outlineType, 0);
}

uint32_t Canvas::FontFace::getGlyphCount() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(glyphCount, 0);
}

uint32_t Canvas::FontFace::getFaceIndex() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceIndex, 0);
}

uint32_t Canvas::FontFace::getFaceFlags() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceFlags, 0);
}

uint32_t Canvas::FontFace::getDiagFlags() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(diagFlags, 0);
}

uint64_t Canvas::FontFace::getFaceUniqueId() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceUniqueId, 0);
}

std::string Canvas::FontFace::getFullName() const TBAG_NOEXCEPT
{
    auto const view = _impl->base.fullNameView();
    return std::string(view.begin(), view.end());
}

std::string Canvas::FontFace::getFamilyName() const TBAG_NOEXCEPT
{
    auto const view = _impl->base.familyNameView();
    return std::string(view.begin(), view.end());
}

std::string Canvas::FontFace::getSubfamilyName() const TBAG_NOEXCEPT
{
    auto const view = _impl->base.subfamilyNameView();
    return std::string(view.begin(), view.end());
}

std::string Canvas::FontFace::getPostScriptName() const TBAG_NOEXCEPT
{
    auto const view = _impl->base.postScriptNameView();
    return std::string(view.begin(), view.end());
}

// ----------------------------------
// Canvas::GlyphBuffer implementation
// ----------------------------------

Canvas::GlyphBuffer::GlyphBuffer() : _impl(std::make_shared<GlyphBufferImpl>())
{
    assert(_impl);
}

bool Canvas::GlyphBuffer::empty() const TBAG_NOEXCEPT
{
    return _impl->base.empty();
}

size_t Canvas::GlyphBuffer::size() const TBAG_NOEXCEPT
{
    return _impl->base.size();
}

uint32_t Canvas::GlyphBuffer::flags() const TBAG_NOEXCEPT
{
    return _impl->base.flags();
}

Canvas::GlyphRun Canvas::GlyphBuffer::getGlyphRun() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.glyphRun());
}

bool Canvas::GlyphBuffer::hasFlag(uint32_t flag) const TBAG_NOEXCEPT
{
    return _impl->base.hasFlag(flag);
}

bool Canvas::GlyphBuffer::hasText() const TBAG_NOEXCEPT
{
    return _impl->base.hasText();
}

bool Canvas::GlyphBuffer::hasGlyphs() const TBAG_NOEXCEPT
{
    return _impl->base.hasGlyphs();
}

bool Canvas::GlyphBuffer::hasInvalidChars() const TBAG_NOEXCEPT
{
    return _impl->base.hasInvalidChars();
}

bool Canvas::GlyphBuffer::hasUndefinedChars() const TBAG_NOEXCEPT
{
    return _impl->base.hasUndefinedChars();
}

bool Canvas::GlyphBuffer::hasInvalidFontData() const TBAG_NOEXCEPT
{
    return _impl->base.hasInvalidFontData();
}

Err Canvas::GlyphBuffer::reset() TBAG_NOEXCEPT
{
    return _call_blend2d_method(reset);
}

Err Canvas::GlyphBuffer::clear() TBAG_NOEXCEPT
{
    return _call_blend2d_method(clear);
}

Err Canvas::GlyphBuffer::setText(void const * data, size_t size, uint32_t encoding) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setText, data, size, encoding);
}

Err Canvas::GlyphBuffer::setLatin1Text(char const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setLatin1Text, data, size);
}

Err Canvas::GlyphBuffer::setUtf8Text(char const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setUtf8Text, data, size);
}

Err Canvas::GlyphBuffer::setUtf8Text(uint8_t const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setUtf8Text, data, size);
}

Err Canvas::GlyphBuffer::setUtf16Text(uint16_t const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setUtf16Text, data, size);
}

Err Canvas::GlyphBuffer::setUtf32Text(uint32_t const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setUtf32Text, data, size);
}

Err Canvas::GlyphBuffer::setWCharText(wchar_t const * data, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setWCharText, data, size);
}

Err Canvas::GlyphBuffer::setGlyphIds(void const * data, intptr_t advance, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setGlyphIds, data, advance, size);
}

// ---------------------------
// Canvas::Font implementation
// ---------------------------

Canvas::Font::Font() : _impl(std::make_shared<FontImpl>())
{
    assert(_impl);
}

Err Canvas::Font::createFromFace(FontFace const & face, float size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(createFromFace, face._impl->base, size);
}

uint32_t Canvas::Font::getFaceType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceType, 0);
}

uint32_t Canvas::Font::getFaceFlags() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(faceFlags, 0);
}

float Canvas::Font::getSize() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(size, 0.0f);
}

int Canvas::Font::getUnitsPerEm() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(unitsPerEm, 0);
}

uint32_t Canvas::Font::getWeight() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(weight, 0);
}

uint32_t Canvas::Font::getStretch() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(stretch, 0);
}

uint32_t Canvas::Font::getStyle() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(style, 0);
}

FontMatrix Canvas::Font::getMatrix() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.matrix());
}

FontMetrics Canvas::Font::getMetrics() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.metrics());
}

FontDesignMetrics Canvas::Font::getDesignMetrics() const TBAG_NOEXCEPT
{
    return _cvt(_impl->base.designMetrics());
}

Err Canvas::Font::shape(GlyphBuffer & gb) const TBAG_NOEXCEPT
{
    return _call_blend2d_method(shape, gb._impl->base);
}

// ---------------------
// Canvas implementation
// ---------------------

Canvas::Canvas() : _impl(nullptr)
{
    // EMPTY.
}

Canvas::Canvas(int width, int height, int channels) : _impl(nullptr)
{
    auto const code = init(width, height, channels);
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

Canvas::~Canvas()
{
    // EMPTY.
}

void Canvas::reset()
{
    _impl.reset();
}

Err Canvas::init(int width, int height, int channels)
{
    if (width <= 0 || height <= 0) {
        return E_ILLARGS;
    }
    if (__get_channels_to_bl_format(channels) == BL_FORMAT_NONE) {
        return E_ILLARGS;
    }
    try {
        _impl = std::make_unique<Impl>(width, height, channels);
        return E_SUCCESS;
    } catch (ErrException const & e) {
        return e.CODE;
    } catch (std::exception const & e) {
        return E_UNKSTDEXCP;
    } catch (...) {
        return E_UNKEXCP;
    }
}

bool Canvas::isNone() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(isNone, false);
}

bool Canvas::equals(Canvas const & obj) const TBAG_NOEXCEPT
{
    return _impl && obj._impl ? _impl->base.equals(obj._impl->base) : false;
}

Size2d Canvas::getTargetSize() const TBAG_NOEXCEPT
{
    return _impl ? _cvt(_impl->base.targetSize()) : Size2d();
}

double Canvas::getTargetWidth() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(targetWidth, 0.0);
}

double Canvas::getTargetHeight() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(targetHeight, 0.0);
}

ContextType Canvas::getContextType() const TBAG_NOEXCEPT
{
    if (!_impl) {
        return ContextType::CT_NONE;
    }
    switch (_impl->base.contextType()) {
    case BL_CONTEXT_TYPE_NONE:
        return ContextType::CT_NONE;
    case BL_CONTEXT_TYPE_DUMMY:
        return ContextType::CT_DUMMY;
    case BL_CONTEXT_TYPE_RASTER:
        return ContextType::CT_RASTER;
    case BL_CONTEXT_TYPE_COUNT:
        return ContextType::CT_COUNT;
    default:
        return ContextType::CT_NONE;
    }
}

Err Canvas::resetContext() TBAG_NOEXCEPT
{
    return _call_blend2d_method(reset);
}

Err Canvas::assign(Canvas const & obj) TBAG_NOEXCEPT
{
    return _call_blend2d_method(assign, obj._impl->base);
}

Err Canvas::begin(Box & box) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(box, image);
    if (isFailure(code)) {
        return code;
    }
    return _call_blend2d_method(begin, image);
}

Err Canvas::end() TBAG_NOEXCEPT
{
    return _call_blend2d_method(end);
}

Err Canvas::flush() TBAG_NOEXCEPT
{
    return _call_blend2d_method(flush, BL_CONTEXT_FLUSH_SYNC);
}

std::size_t Canvas::getSavedStateCount() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(savedStateCount, 0);
}

Err Canvas::save() TBAG_NOEXCEPT
{
    return _call_blend2d_method(save);
}

Err Canvas::save(Cookie & cookie) TBAG_NOEXCEPT
{
    BLContextCookie temp = {};
    temp.reset(cookie.data[0], cookie.data[1]);
    auto const result = _call_blend2d_method(save, temp);
    if (isSuccess(result)) {
        cookie.data[0] = temp.data[0];
        cookie.data[1] = temp.data[1];
    }
    return result;
}

Err Canvas::restore() TBAG_NOEXCEPT
{
    return _call_blend2d_method(restore);
}

Err Canvas::restore(Cookie const & cookie) TBAG_NOEXCEPT
{
    BLContextCookie temp = {};
    temp.reset(cookie.data[0], cookie.data[1]);
    return _call_blend2d_method(restore, temp);
}

Matrix Canvas::getMetaMatrix() const TBAG_NOEXCEPT
{
    return _impl ? _cvt(_impl->base.metaMatrix()) : Matrix();
}

Matrix Canvas::getUserMatrix() const TBAG_NOEXCEPT
{
    return _impl ? _cvt(_impl->base.userMatrix()) : Matrix();
}

Err Canvas::applyMatrixOperator(uint32_t type, void const * data) TBAG_NOEXCEPT
{
    return _call_blend2d_method(_applyMatrixOp, type, data);
}

Err Canvas::setMatrix(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setMatrix, _cvt(m));
}

Err Canvas::resetMatrix() TBAG_NOEXCEPT
{
    return _call_blend2d_method(resetMatrix);
}

Err Canvas::translate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, x, y);
}

Err Canvas::translate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::translate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, _cast(p));
}

Err Canvas::scale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, xy);
}

Err Canvas::scale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, x, y);
}

Err Canvas::scale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::scale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, _cast(p));
}

Err Canvas::skew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, x, y);
}

Err Canvas::skew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, _cast(p));
}

Err Canvas::rotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle);
}

Err Canvas::rotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, x, y);
}

Err Canvas::rotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::rotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, _cast(p));
}

Err Canvas::transform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, _cvt(m));
}

Err Canvas::postTranslate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, x, y);
}

Err Canvas::postTranslate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::postTranslate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, _cast(p));
}

Err Canvas::postScale(double xy) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, xy);
}

Err Canvas::postScale(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, x, y);
}

Err Canvas::postScale(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::postScale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, _cast(p));
}

Err Canvas::postSkew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, x, y);
}

Err Canvas::postSkew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, _cast(p));
}

Err Canvas::postRotate(double angle) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle);
}

Err Canvas::postRotate(double angle, double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, x, y);
}

Err Canvas::postRotate(double angle, Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::postRotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, _cast(p));
}

Err Canvas::postTransform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTransform, _cvt(m));
}

Err Canvas::userToMeta() TBAG_NOEXCEPT
{
    return _call_blend2d_method(userToMeta);
}

uint8_t Canvas::getRenderingQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->base.hints().renderingQuality : 0;
}

uint8_t Canvas::getGradientQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->base.hints().gradientQuality : 0;
}

uint8_t Canvas::getPatternQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->base.hints().patternQuality : 0;
}

Err Canvas::setRenderingQuality(uint32_t value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setRenderingQuality, value);
}

Err Canvas::setGradientQuality(uint32_t value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setGradientQuality, value);
}

Err Canvas::setPatternQuality(uint32_t value) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setPatternQuality, value);
}

uint32_t Canvas::getFlattenMode() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(flattenMode, 0);
}

Err Canvas::setFlattenMode(uint32_t mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFlattenMode, mode);
}

double Canvas::getFlattenTolerance() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(flattenTolerance, 0.0);
}

Err Canvas::setFlattenTolerance(double tolerance) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFlattenTolerance, tolerance);
}

BlendMode Canvas::getCompositionOperator() const TBAG_NOEXCEPT
{
    return _impl ? __get_bl_comp_op_to_blend_mode(_impl->base.compOp()) : BlendMode::BM_SRC_OVER;
}

Err Canvas::setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setCompOp, __get_blend_mode_to_bl_comp_op(mode));
}

double Canvas::getGlobalAlpha() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(globalAlpha, 0.0);
}

Err Canvas::setGlobalAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setGlobalAlpha, alpha);
}

double Canvas::getFillAlpha() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(fillAlpha, 0.0);
}

Err Canvas::setFillAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillAlpha, alpha);
}

uint32_t Canvas::getFillStyleType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(fillStyleType, 0);
}

Err Canvas::getFillStyle(Rgb32 & out) const TBAG_NOEXCEPT
{
    BLRgba32 temp = {};
    auto const code = _call_blend2d_method(getFillStyle, temp);
    if (isSuccess(code)) {
        out = _cvt(temp);
    }
    return code;
}

Err Canvas::setFillStyle(Channel r, Channel g, Channel b, Channel a) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillStyle, (BLRgba32(r, g, b, a)));
}

Err Canvas::setFillStyle(Rgb32 const & color) TBAG_NOEXCEPT
{
    return setFillStyle(color.r, color.g, color.b, color.a);
}

Err Canvas::setFillStyle(Gradient const & gradient) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillStyle, gradient._impl->base);
}

Err Canvas::setFillStyle(Pattern const & pattern) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillStyle, pattern._impl->base);
}

uint32_t Canvas::getFillRule() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(fillRule, 0);
}

Err Canvas::setFillRule(uint32_t rule) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillRule, rule);
}

double Canvas::getStrokeAlpha() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeAlpha, 0.0);
}

Err Canvas::setStrokeAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeAlpha, alpha);
}

uint32_t Canvas::getStrokeStyleType() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeStyleType, 0);
}

Err Canvas::getStrokeStyle(Rgb32 & out) const TBAG_NOEXCEPT
{
    BLRgba32 temp = {};
    auto const code = _call_blend2d_method(getStrokeStyle, temp);
    if (isSuccess(code)) {
        out = _cvt(temp);
    }
    return code;
}

Err Canvas::setStrokeStyle(Channel r, Channel g, Channel b, Channel a) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeStyle, (BLRgba32(r, g, b, a)));
}

Err Canvas::setStrokeStyle(Rgb32 const & color) TBAG_NOEXCEPT
{
    return setStrokeStyle(color.r, color.g, color.b, color.a);
}

Err Canvas::setStrokeStyle(Pattern const & pattern) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeStyle, pattern._impl->base);
}

Err Canvas::setStrokeStyle(Gradient const & gradient) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeStyle, gradient._impl->base);
}

double Canvas::getStrokeWidth() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeWidth, 0.0);
}

double Canvas::getStrokeMiterLimit() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeMiterLimit, 0.0);
}

uint32_t Canvas::getStrokeJoin() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeJoin, 0);
}

uint32_t Canvas::getStrokeStartCap() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeStartCap, 0);
}

uint32_t Canvas::getStrokeEndCap() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeEndCap, 0);
}

double Canvas::getStrokeDashOffset() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeDashOffset, 0.0);
}

std::vector<double> Canvas::getStrokeDashArray() const TBAG_NOEXCEPT
{
    if (!_impl) {
        return {};
    }
    auto const arr = _impl->base.strokeDashArray();
    auto const size = arr.size();
    std::vector<double> result(size);
    for (auto i = 0; i < size; ++i) {
        result[i] = arr[i];
    }
    return result;
}

uint32_t Canvas::getStrokeTransformOrder() const TBAG_NOEXCEPT
{
    return _call_blend2d_getter(strokeTransformOrder, 0);
}

Err Canvas::setStrokeWidth(double width) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeWidth, width);
}

Err Canvas::setStrokeMiterLimit(double miter_limit) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeMiterLimit, miter_limit);
}

Err Canvas::setStrokeJoin(uint32_t stroke_join) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeJoin, stroke_join);
}

Err Canvas::setStrokeCap(uint32_t type, uint32_t stroke_cap) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeCap, type, stroke_cap);
}

Err Canvas::setStrokeStartCap(uint32_t stroke_cap) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeStartCap, stroke_cap);
}

Err Canvas::setStrokeEndCap(uint32_t stroke_cap) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeEndCap, stroke_cap);
}

Err Canvas::setStrokeCaps(uint32_t stroke_cap) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeCaps, stroke_cap);
}

Err Canvas::setStrokeDashOffset(double dash_offset) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeDashOffset, dash_offset);
}

Err Canvas::setStrokeDashArray(std::vector<double> const & dash_array) TBAG_NOEXCEPT
{
    BLArray<double> arr;
    auto const create_result = arr.createFromData((double*)dash_array.data(),
                                                  dash_array.size(),
                                                  dash_array.size(),
                                                  BL_DATA_ACCESS_READ);
    if (create_result != BL_SUCCESS) {
        return convertBLResultToErr(create_result);
    }
    return _call_blend2d_method(setStrokeDashArray, arr);
}

Err Canvas::setStrokeTransformOrder(uint32_t transform_order) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeTransformOrder, transform_order);
}

Err Canvas::restoreClipping() TBAG_NOEXCEPT
{
    return _call_blend2d_method(restoreClipping);
}

Err Canvas::clipToRect(double x, double y, double w, double h) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clipToRect, x, y, w, h);
}

Err Canvas::clipToRect(Rect2d const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clipToRect, _cvt(rect));
}

Err Canvas::clipToRect(Rect2i const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clipToRect, _cvt(rect));
}

Err Canvas::clearAll() TBAG_NOEXCEPT
{
    return _call_blend2d_method(clearAll);
}

Err Canvas::clearRect(double x, double y, double w, double h) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clearRect, x, y, w, h);
}

Err Canvas::clearRect(Rect2d const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clearRect, _cvt(rect));
}

Err Canvas::clearRect(Rect2i const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clearRect, _cvt(rect));
}

Err Canvas::fillAll() TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillAll);
}

Err Canvas::fillBox(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillBox, x0, y0, x1, y1);
}

Err Canvas::fillRect(double x, double y, double w, double h) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRect, x, y, w, h);
}

Err Canvas::fillCircle(double cx, double cy, double r) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillCircle, cx, cy, r);
}

Err Canvas::fillEllipse(double cx, double cy, double rx, double ry) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillEllipse, cx, cy, rx, ry);
}

Err Canvas::fillRoundRect(double x, double y, double w, double h, double r) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRoundRect, x, y, w, h, r);
}

Err Canvas::fillRoundRect(double x, double y, double w, double h, double rx, double ry) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRoundRect, x, y, w, h, rx, ry);
}

Err Canvas::fillChord(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillChord, cx, cy, r, start, sweep);
}

Err Canvas::fillChord(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillChord, cx, cy, rx, ry, start, sweep);
}

Err Canvas::fillPie(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPie, cx, cy, r, start, sweep);
}

Err Canvas::fillPie(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPie, cx, cy, rx, ry, start, sweep);
}

Err Canvas::fillTriangle(double x0, double y0, double x1, double y1, double x2, double y2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillTriangle, x0, y0, x1, y1, x2, y2);
}

Err Canvas::fillPolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPolygon, _cast(poly), n);
}

Err Canvas::fillPolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPolygon, _cast(poly), n);
}

Err Canvas::fillRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, _cast(data), n);
}

Err Canvas::fillRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, _cast(data), n);
}

Err Canvas::fillPath(Path const & path) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPath, path._impl->base);
}

Err Canvas::fillUtf8Text(Point2i const & dst, Font const & font, char const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf8Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillUtf8Text(Point2d const & dst, Font const & font, char const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf8Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillUtf16Text(Point2i const & dst, Font const & font, uint16_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf16Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillUtf16Text(Point2d const & dst, Font const & font, uint16_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf16Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillUtf32Text(Point2i const & dst, Font const & font, uint32_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf32Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillUtf32Text(Point2d const & dst, Font const & font, uint32_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillUtf32Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::fillGlyphRun(Point2i const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillGlyphRun, _cast(dst), font._impl->base, _cvt(glyph_run));
}

Err Canvas::fillGlyphRun(Point2d const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillGlyphRun, _cast(dst), font._impl->base, _cvt(glyph_run));
}

Err Canvas::strokeBox(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeBox, x0, y0, x1, y1);
}

Err Canvas::strokeRect(double x, double y, double w, double h) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRect, x, y, w, h);
}

Err Canvas::strokeLine(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeLine, x0, y0, x1, y1);
}

Err Canvas::strokeCircle(double cx, double cy, double r) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeCircle, cx, cy, r);
}

Err Canvas::strokeEllipse(double cx, double cy, double rx, double ry) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeEllipse, cx, cy, rx, ry);
}

Err Canvas::strokeRoundRect(double x, double y, double w, double h, double r) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRoundRect, x, y, w, h, r);
}

Err Canvas::strokeRoundRect(double x, double y, double w, double h, double rx, double ry) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRoundRect, x, y, w, h, rx, ry);
}

Err Canvas::strokeArc(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeArc, cx, cy, r, start, sweep);
}

Err Canvas::strokeArc(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeArc, cx, cy, rx, ry, start, sweep);
}

Err Canvas::strokeChord(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeChord, cx, cy, r, start, sweep);
}

Err Canvas::strokeChord(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeChord, cx, cy, rx, ry, start, sweep);
}

Err Canvas::strokePie(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePie, cx, cy, r, start, sweep);
}

Err Canvas::strokePie(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePie, cx, cy, rx, ry, start, sweep);
}

Err Canvas::strokeTriangle(double x0, double y0, double x1, double y1, double x2, double y2) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeTriangle, x0, y0, x1, y1, x2, y2);
}

Err Canvas::strokePolyline(Point2d const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolyline, _cast(poly), n);
}

Err Canvas::strokePolyline(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolyline, _cast(poly), n);
}

Err Canvas::strokePolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, _cast(poly), n);
}

Err Canvas::strokePolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, _cast(poly), n);
}

Err Canvas::strokeRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, _cast(data), n);
}

Err Canvas::strokeRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, _cast(data), n);
}

Err Canvas::strokePath(Path const & path) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePath, path._impl->base);
}

Err Canvas::strokeUtf8Text(Point2i const & dst, Font const & font, char const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf8Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeUtf8Text(Point2d const & dst, Font const & font, char const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf8Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeUtf16Text(Point2i const & dst, Font const & font, uint16_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf16Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeUtf16Text(Point2d const & dst, Font const & font, uint16_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf16Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeUtf32Text(Point2i const & dst, Font const & font, uint32_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf32Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeUtf32Text(Point2d const & dst, Font const & font, uint32_t const * text, size_t size) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeUtf32Text, _cast(dst), font._impl->base, text, size);
}

Err Canvas::strokeGlyphRun(Point2i const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeGlyphRun, _cast(dst), font._impl->base, _cvt(glyph_run));
}

Err Canvas::strokeGlyphRun(Point2d const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeGlyphRun, _cast(dst), font._impl->base, _cvt(glyph_run));
}

Err Canvas::blitImage(Point2d const & dst, Box const & src) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src) : E_NREADY;
}

Err Canvas::blitImage(Point2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    return _impl ?_impl->blit_image(dst, src, src_area) : E_NREADY;
}

Err Canvas::blitImage(Point2i const & dst, Box const & src) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src) : E_NREADY;
}

Err Canvas::blitImage(Point2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src, src_area) : E_NREADY;
}

Err Canvas::blitImage(Rect2d const & dst, Box const & src) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src) : E_NREADY;
}

Err Canvas::blitImage(Rect2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src, src_area) : E_NREADY;
}

Err Canvas::blitImage(Rect2i const & dst, Box const & src) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src) : E_NREADY;
}

Err Canvas::blitImage(Rect2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    return _impl ? _impl->blit_image(dst, src, src_area) : E_NREADY;
}

Err Canvas::toBox(Box & output) const
{
    return _impl ? __create_bl_image(_impl->image, output) : E_NREADY;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

