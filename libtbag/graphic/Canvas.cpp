/**
 * @file   Canvas.cpp
 * @brief  Canvas class implementation.
 * @author zer0
 * @date   2020-01-11
 */

#include <libtbag/graphic/Canvas.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/util/OffsetOf.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <blend2d.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

using Point2d = Canvas::Point2d;
using Point2i = Canvas::Point2i;

static_assert(sizeof(BLPoint) == sizeof(Point2d) &&
              alignof(BLPoint) == alignof(Point2d) &&
              TBAG_OFFSET_OF(BLPoint, x) == TBAG_OFFSET_OF(Point2d, x) &&
              TBAG_OFFSET_OF(BLPoint, y) == TBAG_OFFSET_OF(Point2d, y),
              "BLPoint and Point2d must have the same structure.");
static_assert(sizeof(BLPointI) == sizeof(Point2i) &&
              alignof(BLPointI) == alignof(Point2i) &&
              TBAG_OFFSET_OF(BLPointI, x) == TBAG_OFFSET_OF(Point2i, x) &&
              TBAG_OFFSET_OF(BLPointI, y) == TBAG_OFFSET_OF(Point2i, y),
              "BLPointI and Point2i must have the same structure.");

using Rect2d = Canvas::Rect2d;
using Rect2i = Canvas::Rect2i;

static_assert(sizeof(BLRect) == sizeof(Rect2d) &&
              alignof(BLRect) == alignof(Rect2d) &&
              TBAG_OFFSET_OF(BLRect, x) == TBAG_OFFSET_OF(Rect2d, x) &&
              TBAG_OFFSET_OF(BLRect, y) == TBAG_OFFSET_OF(Rect2d, y) &&
              TBAG_OFFSET_OF(BLRect, w) == TBAG_OFFSET_OF(Rect2d, width) &&
              TBAG_OFFSET_OF(BLRect, h) == TBAG_OFFSET_OF(Rect2d, height),
              "BLRect and Rect2d must have the same structure.");
static_assert(sizeof(BLRectI) == sizeof(Rect2i) &&
              alignof(BLRectI) == alignof(Rect2i) &&
              TBAG_OFFSET_OF(BLRectI, x) == TBAG_OFFSET_OF(Rect2i, x) &&
              TBAG_OFFSET_OF(BLRectI, y) == TBAG_OFFSET_OF(Rect2i, y) &&
              TBAG_OFFSET_OF(BLRectI, w) == TBAG_OFFSET_OF(Rect2i, width) &&
              TBAG_OFFSET_OF(BLRectI, h) == TBAG_OFFSET_OF(Rect2i, height),
              "BLRectI and Rect2i must have the same structure.");

static uint32_t __get_channels_to_bl_format(int channels) TBAG_NOEXCEPT
{
    switch (channels) {
    case 3: return BL_FORMAT_XRGB32;
    case 4: return BL_FORMAT_PRGB32;
    default: return BL_FORMAT_NONE;
    }
}

static uint32_t __get_blend_mode_to_bl_comp_op(Canvas::BlendMode mode) TBAG_NOEXCEPT
{
    // clang-format off
    switch (mode) {
    case Canvas::BlendMode::BM_SRC_OVER    : return BL_COMP_OP_SRC_OVER;
    case Canvas::BlendMode::BM_SRC_COPY    : return BL_COMP_OP_SRC_COPY;
    case Canvas::BlendMode::BM_SRC_IN      : return BL_COMP_OP_SRC_IN;
    case Canvas::BlendMode::BM_SRC_OUT     : return BL_COMP_OP_SRC_OUT;
    case Canvas::BlendMode::BM_SRC_ATOP    : return BL_COMP_OP_SRC_ATOP;
    case Canvas::BlendMode::BM_DST_OVER    : return BL_COMP_OP_DST_OVER;
    case Canvas::BlendMode::BM_DST_COPY    : return BL_COMP_OP_DST_COPY;
    case Canvas::BlendMode::BM_DST_IN      : return BL_COMP_OP_DST_IN;
    case Canvas::BlendMode::BM_DST_OUT     : return BL_COMP_OP_DST_OUT;
    case Canvas::BlendMode::BM_DST_ATOP    : return BL_COMP_OP_DST_ATOP;
    case Canvas::BlendMode::BM_XOR         : return BL_COMP_OP_XOR;
    case Canvas::BlendMode::BM_CLEAR       : return BL_COMP_OP_CLEAR;
    case Canvas::BlendMode::BM_PLUS        : return BL_COMP_OP_PLUS;
    case Canvas::BlendMode::BM_MINUS       : return BL_COMP_OP_MINUS;
    case Canvas::BlendMode::BM_MULTIPLY    : return BL_COMP_OP_MULTIPLY;
    case Canvas::BlendMode::BM_SCREEN      : return BL_COMP_OP_SCREEN;
    case Canvas::BlendMode::BM_OVERLAY     : return BL_COMP_OP_OVERLAY;
    case Canvas::BlendMode::BM_DARKEN      : return BL_COMP_OP_DARKEN;
    case Canvas::BlendMode::BM_LIGHTEN     : return BL_COMP_OP_LIGHTEN;
    case Canvas::BlendMode::BM_COLOR_DODGE : return BL_COMP_OP_COLOR_DODGE;
    case Canvas::BlendMode::BM_COLOR_BURN  : return BL_COMP_OP_COLOR_BURN;
    case Canvas::BlendMode::BM_LINEAR_BURN : return BL_COMP_OP_LINEAR_BURN;
    case Canvas::BlendMode::BM_LINEAR_LIGHT: return BL_COMP_OP_LINEAR_LIGHT;
    case Canvas::BlendMode::BM_PIN_LIGHT   : return BL_COMP_OP_PIN_LIGHT;
    case Canvas::BlendMode::BM_HARD_LIGHT  : return BL_COMP_OP_HARD_LIGHT;
    case Canvas::BlendMode::BM_SOFT_LIGHT  : return BL_COMP_OP_SOFT_LIGHT;
    case Canvas::BlendMode::BM_DIFFERENCE  : return BL_COMP_OP_DIFFERENCE;
    case Canvas::BlendMode::BM_EXCLUSION   : return BL_COMP_OP_EXCLUSION;
    }
    // clang-format on
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
    BLContext context;

    Impl(int w, int h, int c)
            : width(w), height(h), channels(c),
              image(w, h, __get_channels_to_bl_format(c)),
              context(image)
    { /* EMPTY. */ }
    ~Impl()
    { /* EMPTY. */ }
};

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

Err Canvas::convertBLResultToErr(uint32_t code) TBAG_NOEXCEPT
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

#ifndef _call_blend2d_method
#define _call_blend2d_method(func, ...) \
    ((_impl) ? (convertBLResultToErr(_impl->context.func(__VA_ARGS__))) : (E_NREADY))
#endif

Err Canvas::end() TBAG_NOEXCEPT
{
    return _call_blend2d_method(end);
}

Err Canvas::setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setCompOp, __get_blend_mode_to_bl_comp_op(mode));
}

Err Canvas::setFillStyle(Channel r, Channel g, Channel b, Channel a) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillStyle, (BLRgba32(r, g, b, a)));
}

Err Canvas::setFillStyle(Rgb32 const & color) TBAG_NOEXCEPT
{
    return setFillStyle(color.r, color.g, color.b, color.a);
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
    return _call_blend2d_method(fillPolygon, (BLPoint const *)poly, n);
}

Err Canvas::fillPolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPolygon, (BLPointI const *)poly, n);
}

Err Canvas::fillRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, (BLRect const *)data, n);
}

Err Canvas::fillRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, (BLRectI const *)data, n);
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
    return _call_blend2d_method(strokePolyline, (BLPoint const *)poly, n);
}

Err Canvas::strokePolyline(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolyline, (BLPointI const *)poly, n);
}

Err Canvas::strokePolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, (BLPoint const *)poly, n);
}

Err Canvas::strokePolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, (BLPointI const *)poly, n);
}

Err Canvas::strokeRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, (BLRect const *)data, n);
}

Err Canvas::strokeRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, (BLRectI const *)data, n);
}

static Err __create_rgba_image(Canvas::Box const & box, BLImage & dest)
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
    return Canvas::convertBLResultToErr(dest.createFromData(width, height, format, (void*)box.data(), stride));
}

Err Canvas::blitImage(Point2d const & dst, Box const & src) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLPoint(dst.x, dst.y);
    auto const blit_result = _impl->context.blitImage(bl_dst, image);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Point2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLPoint(dst.x, dst.y);
    auto const bl_src_area = BLRectI(src_area.x, src_area.y, src_area.width, src_area.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image, bl_src_area);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Point2i const & dst, Box const & src) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLPointI(dst.x, dst.y);
    auto const blit_result = _impl->context.blitImage(bl_dst, image);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Point2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLPointI(dst.x, dst.y);
    auto const bl_src_area = BLRectI(src_area.x, src_area.y, src_area.width, src_area.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image, bl_src_area);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Rect2d const & dst, Box const & src) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLRect(dst.x, dst.y, dst.width, dst.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Rect2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLRect(dst.x, dst.y, dst.width, dst.height);
    auto const bl_src_area = BLRectI(src_area.x, src_area.y, src_area.width, src_area.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image, bl_src_area);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Rect2i const & dst, Box const & src) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLRectI(dst.x, dst.y, dst.width, dst.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image);
    return convertBLResultToErr(blit_result);
}

Err Canvas::blitImage(Rect2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT
{
    BLImage image;
    auto const code = __create_rgba_image(src, image);
    if (isFailure(code)) {
        return code;
    }
    auto const bl_dst = BLRectI(dst.x, dst.y, dst.width, dst.height);
    auto const bl_src_area = BLRectI(src_area.x, src_area.y, src_area.width, src_area.height);
    auto const blit_result = _impl->context.blitImage(bl_dst, image, bl_src_area);
    return convertBLResultToErr(blit_result);
}

Err Canvas::toBox(libtbag::box::Box & output) const
{
    if (!_impl) {
        return E_NREADY;
    }

    BLImageData data = {};
    auto const obtain_code = convertBLResultToErr(_impl->image.getData(&data));
    if (isFailure(obtain_code)) {
        return obtain_code;
    }

    auto const & width = _impl->width;
    auto const & height = _impl->height;
    auto const & channels = _impl->channels;
    if (__get_channels_to_bl_format(channels) == BL_FORMAT_NONE) {
        return E_ILLARGS;
    }

    auto const resize_code = output.resize<std::uint8_t>(height, width, channels);
    if (isFailure(resize_code)) {
        return resize_code;
    }

    if (channels == 3) {
        auto const * source = static_cast<std::uint8_t const *>(data.pixelData);
        auto * destination = output.data<std::uint8_t>();
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
        memcpy(output.data(), data.pixelData, (height*width*channels));
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_INACCESSIBLE_BLOCK;
    }
    return E_SUCCESS;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

