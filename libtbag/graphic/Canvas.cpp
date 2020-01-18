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

using ContextType = Canvas::ContextType;
using BlendMode = Canvas::BlendMode;

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

using Matrix = Canvas::Matrix;
using Box = Canvas::Box;

// Tbag -> Blend2D

static BLPoint __convert_bl_structure(Point2d const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static BLPointI __convert_bl_structure(Point2i const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static BLRect __convert_bl_structure(Rect2d const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.width, v.height};
}

static BLRectI __convert_bl_structure(Rect2i const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.width, v.height};
}

static BLSize __convert_bl_structure(Size2d const & v) TBAG_NOEXCEPT
{
    return {v.width, v.height};
}

static BLSizeI __convert_bl_structure(Size2i const & v) TBAG_NOEXCEPT
{
    return {v.width, v.height};
}

static BLRgba32 __convert_bl_structure(Rgb32 const & v) TBAG_NOEXCEPT
{
    return {v.r, v.g, v.b, v.a};
}

static BLRgba32 __convert_bl_structure(Rgb24 const & v) TBAG_NOEXCEPT
{
    return {v.r, v.g, v.b, 0xFF};
}

static BLMatrix2D __convert_bl_structure(Matrix const & v) TBAG_NOEXCEPT
{
    return {v.array[0], v.array[1], v.array[2], v.array[3], v.array[4], v.array[5]};
}

// Blend2D -> Tbag

static Point2d __convert_bl_structure(BLPoint const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static Point2i __convert_bl_structure(BLPointI const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y};
}

static Rect2d __convert_bl_structure(BLRect const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.w, v.h};
}

static Rect2i __convert_bl_structure(BLRectI const & v) TBAG_NOEXCEPT
{
    return {v.x, v.y, v.w, v.h};
}

static Size2d __convert_bl_structure(BLSize const & v) TBAG_NOEXCEPT
{
    return {v.w, v.h};
}

static Size2i __convert_bl_structure(BLSizeI const & v) TBAG_NOEXCEPT
{
    return {v.w, v.h};
}

static Rgb32 __convert_bl_structure(BLRgba32 const & v) TBAG_NOEXCEPT
{
    return Rgb32(v.r, v.g, v.b, v.a);
}

static Matrix __convert_bl_structure(BLMatrix2D const & v) TBAG_NOEXCEPT
{
    return {v.m00, v.m01, v.m10, v.m11, v.m20, v.m21};
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
template <> struct _BLCast<BLPoint   > : public BLStructureConverterImpl<BLPoint   , Point2d> { using type = tbtype; };
template <> struct _BLCast<BLPointI  > : public BLStructureConverterImpl<BLPointI  , Point2i> { using type = tbtype; };
template <> struct _BLCast<BLRect    > : public BLStructureConverterImpl<BLRect    , Rect2d > { using type = tbtype; };
template <> struct _BLCast<BLRectI   > : public BLStructureConverterImpl<BLRectI   , Rect2i > { using type = tbtype; };
template <> struct _BLCast<BLSize    > : public BLStructureConverterImpl<BLSize    , Size2d > { using type = tbtype; };
template <> struct _BLCast<BLSizeI   > : public BLStructureConverterImpl<BLSizeI   , Size2i > { using type = tbtype; };
template <> struct _BLCast<BLMatrix2D> : public BLStructureConverterImpl<BLMatrix2D, Matrix > { using type = tbtype; };
template <> struct _BLCast<Point2d> : public BLStructureConverterImpl<BLPoint   , Point2d> { using type = bltype; };
template <> struct _BLCast<Point2i> : public BLStructureConverterImpl<BLPointI  , Point2i> { using type = bltype; };
template <> struct _BLCast<Rect2d > : public BLStructureConverterImpl<BLRect    , Rect2d > { using type = bltype; };
template <> struct _BLCast<Rect2i > : public BLStructureConverterImpl<BLRectI   , Rect2i > { using type = bltype; };
template <> struct _BLCast<Size2d > : public BLStructureConverterImpl<BLSize    , Size2d > { using type = bltype; };
template <> struct _BLCast<Size2i > : public BLStructureConverterImpl<BLSizeI   , Size2i > { using type = bltype; };
template <> struct _BLCast<Matrix > : public BLStructureConverterImpl<BLMatrix2D, Matrix > { using type = bltype; };
// clang-format on

template <typename T>
static auto __bl_cast(T p) TBAG_NOEXCEPT
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

    template <typename PointT>
    Err blit_image(PointT p, Canvas::Box const & src)
    {
        BLImage temp;
        auto const code = __create_rgba_image(src, temp);
        if (isFailure(code)) {
            return code;
        }
        return convertBLResultToErr(context.blitImage(__convert_bl_structure(p), temp));
    }

    template <typename PointT, typename RectT>
    Err blit_image(PointT p, Canvas::Box const & src, RectT r)
    {
        BLImage temp;
        auto const code = __create_rgba_image(src, temp);
        if (isFailure(code)) {
            return code;
        }
        return convertBLResultToErr(context.blitImage(__convert_bl_structure(p), temp, __convert_bl_structure(r)));
    }
};

// clang-format off
struct Canvas::ImageImpl    : private Noncopyable { BLImage    image;    };
struct Canvas::PatternImpl  : private Noncopyable { BLPattern  pattern;  };
struct Canvas::GradientImpl : private Noncopyable { BLGradient gradient; };
struct Canvas::PathImpl     : private Noncopyable { BLPath     path;     };
struct Canvas::FontImpl     : private Noncopyable { BLFont     font;     };
struct Canvas::GlyphImpl    : private Noncopyable { BLGlyphRun glyph;    };
// clang-format on

// ----------------------------
// Canvas::Image implementation
// ----------------------------

Canvas::Image::Image() : _impl(std::make_shared<ImageImpl>())
{
    assert(_impl);
}

Err Canvas::Image::create(int w, int h, uint32_t format) TBAG_NOEXCEPT
{
    assert(_impl);
    return convertBLResultToErr(_impl->image.create(w, h, format));
}

int Canvas::Image::width() const TBAG_NOEXCEPT
{
    assert(_impl);
    return _impl->image.width();
}

int Canvas::Image::height() const TBAG_NOEXCEPT
{
    assert(_impl);
    return _impl->image.height();
}

uint32_t Canvas::Image::format() const TBAG_NOEXCEPT
{
    assert(_impl);
    return _impl->image.format();
}

Err Canvas::Image::readFromFile(char const * file_name) TBAG_NOEXCEPT
{
    assert(_impl);
    return convertBLResultToErr(_impl->image.readFromFile(file_name));
}

Err Canvas::Image::readFromData(void const * data, size_t size) TBAG_NOEXCEPT
{
    assert(_impl);
    return convertBLResultToErr(_impl->image.readFromData(data, size));
}

Err Canvas::Image::readFromBox(Box const & box)
{
    assert(_impl);
    return __create_rgba_image(box, _impl->image);
}

Err Canvas::Image::writeToBox(Box & box) const
{
    assert(_impl);
    return __create_bl_image(_impl->image, box);
}

// ------------------------------
// Canvas::Pattern implementation
// ------------------------------

Canvas::Pattern::Pattern() : _impl(std::make_shared<PatternImpl>())
{
    // EMPTY.
}

// -------------------------------
// Canvas::Gradient implementation
// -------------------------------

Canvas::Gradient::Gradient() : _impl(std::make_shared<GradientImpl>())
{
    // EMPTY.
}

// ---------------------------
// Canvas::Path implementation
// ---------------------------

Canvas::Path::Path() : _impl(std::make_shared<PathImpl>())
{
    // EMPTY.
}

// ---------------------------
// Canvas::Font implementation
// ---------------------------

Canvas::Font::Font() : _impl(std::make_shared<FontImpl>())
{
    // EMPTY.
}

// ----------------------------
// Canvas::Glyph implementation
// ----------------------------

Canvas::Glyph::Glyph() : _impl(std::make_shared<GlyphImpl>())
{
    // EMPTY.
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
    return _impl ? _impl->context.isNone() : false;
}

bool Canvas::equals(Canvas const & obj) const TBAG_NOEXCEPT
{
    return _impl && obj._impl ? _impl->context.equals(obj._impl->context) : false;
}

Size2d Canvas::getTargetSize() const TBAG_NOEXCEPT
{
    return _impl ? __convert_bl_structure(_impl->context.targetSize()) : Size2d();
}

double Canvas::getTargetWidth() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.targetWidth() : 0.0;
}

double Canvas::getTargetHeight() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.targetHeight() : 0.0;
}

ContextType Canvas::getContextType() const TBAG_NOEXCEPT
{
    if (!_impl) {
        return ContextType::CT_NONE;
    }
    switch (_impl->context.contextType()) {
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

#ifndef _call_blend2d_method
#define _call_blend2d_method(func, ...) \
    ((_impl) ? (convertBLResultToErr(_impl->context.func(__VA_ARGS__))) : (E_NREADY))
#endif

Err Canvas::resetContext() TBAG_NOEXCEPT
{
    return _call_blend2d_method(reset);
}

Err Canvas::assign(Canvas const & obj) TBAG_NOEXCEPT
{
    return _call_blend2d_method(assign, obj._impl->context);
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
    return _impl ? _impl->context.savedStateCount() : 0;
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
    return _impl ? __convert_bl_structure(_impl->context.metaMatrix()) : Matrix();
}

Matrix Canvas::getUserMatrix() const TBAG_NOEXCEPT
{
    return _impl ? __convert_bl_structure(_impl->context.userMatrix()) : Matrix();
}

Err Canvas::applyMatrixOperator(uint32_t type, void const * data) TBAG_NOEXCEPT
{
    return _call_blend2d_method(_applyMatrixOp, type, data);
}

Err Canvas::setMatrix(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setMatrix, __bl_cast(m));
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
    return _call_blend2d_method(translate, __bl_cast(p));
}

Err Canvas::translate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(translate, __bl_cast(p));
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
    return _call_blend2d_method(scale, __bl_cast(p));
}

Err Canvas::scale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(scale, __bl_cast(p));
}

Err Canvas::skew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, x, y);
}

Err Canvas::skew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(skew, __bl_cast(p));
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
    return _call_blend2d_method(rotate, angle, __bl_cast(p));
}

Err Canvas::rotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(rotate, angle, __bl_cast(p));
}

Err Canvas::transform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(transform, __bl_cast(m));
}

Err Canvas::postTranslate(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, x, y);
}

Err Canvas::postTranslate(Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, __bl_cast(p));
}

Err Canvas::postTranslate(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTranslate, __bl_cast(p));
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
    return _call_blend2d_method(postScale, __bl_cast(p));
}

Err Canvas::postScale(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postScale, __bl_cast(p));
}

Err Canvas::postSkew(double x, double y) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, x, y);
}

Err Canvas::postSkew(Point2d const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postSkew, __bl_cast(p));
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
    return _call_blend2d_method(postRotate, angle, __bl_cast(p));
}

Err Canvas::postRotate(double angle, Point2i const & p) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postRotate, angle, __bl_cast(p));
}

Err Canvas::postTransform(Matrix const & m) TBAG_NOEXCEPT
{
    return _call_blend2d_method(postTransform, __bl_cast(m));
}

Err Canvas::userToMeta() TBAG_NOEXCEPT
{
    return _call_blend2d_method(userToMeta);
}

uint8_t Canvas::getRenderingQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.hints().renderingQuality : 0;
}

uint8_t Canvas::getGradientQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.hints().gradientQuality : 0;
}

uint8_t Canvas::getPatternQuality() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.hints().patternQuality : 0;
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
    return _impl ? _impl->context.flattenMode() : 0;
}

Err Canvas::setFlattenMode(uint32_t mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFlattenMode, mode);
}

double Canvas::getFlattenTolerance() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.flattenTolerance() : 0.0;
}

Err Canvas::setFlattenTolerance(double tolerance) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFlattenTolerance, tolerance);
}

BlendMode Canvas::getCompositionOperator() const TBAG_NOEXCEPT
{
    return _impl ? __get_bl_comp_op_to_blend_mode(_impl->context.compOp()) : BlendMode::BM_SRC_OVER;
}

Err Canvas::setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setCompOp, __get_blend_mode_to_bl_comp_op(mode));
}

double Canvas::getGlobalAlpha() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.globalAlpha() : 0.0;
}

Err Canvas::setGlobalAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setGlobalAlpha, alpha);
}

double Canvas::getFillAlpha() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.fillAlpha() : 0.0;
}

Err Canvas::setFillAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillAlpha, alpha);
}

uint32_t Canvas::getFillStyleType() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.fillStyleType() : 0;
}

Err Canvas::getFillStyle(Rgb32 & out) const TBAG_NOEXCEPT
{
    BLRgba32 temp = {};
    auto const code = _call_blend2d_method(getFillStyle, temp);
    if (isSuccess(code)) {
        out = __convert_bl_structure(temp);
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
    return E_ENOSYS;
}

Err Canvas::setFillStyle(Pattern const & pattern) TBAG_NOEXCEPT
{
    return E_ENOSYS;
}

uint32_t Canvas::getFillRule() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.fillRule() : 0;
}

Err Canvas::setFillRule(uint32_t rule) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setFillRule, rule);
}

double Canvas::getStrokeAlpha() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeAlpha() : 0.0;
}

Err Canvas::setStrokeAlpha(double alpha) TBAG_NOEXCEPT
{
    return _call_blend2d_method(setStrokeAlpha, alpha);
}

uint32_t Canvas::getStrokeStyleType() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeStyleType() : 0;
}

Err Canvas::getStrokeStyle(Rgb32 & out) const TBAG_NOEXCEPT
{
    BLRgba32 temp = {};
    auto const code = _call_blend2d_method(getStrokeStyle, temp);
    if (isSuccess(code)) {
        out = __convert_bl_structure(temp);
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
    return E_ENOSYS;
}

Err Canvas::setStrokeStyle(Gradient const & gradient) TBAG_NOEXCEPT
{
    return E_ENOSYS;
}

double Canvas::getStrokeWidth() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeWidth() : 0.0;
}

double Canvas::getStrokeMiterLimit() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeMiterLimit() : 0.0;
}

uint32_t Canvas::getStrokeJoin() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeJoin() : 0;
}

uint32_t Canvas::getStrokeStartCap() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeStartCap() : 0;
}

uint32_t Canvas::getStrokeEndCap() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeEndCap() : 0;
}

double Canvas::getStrokeDashOffset() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeDashOffset() : 0.0;
}

std::vector<double> Canvas::getStrokeDashArray() const TBAG_NOEXCEPT
{
    if (!_impl) {
        return {};
    }
    auto const arr = _impl->context.strokeDashArray();
    auto const size = arr.size();
    std::vector<double> result(size);
    for (auto i = 0; i < size; ++i) {
        result[i] = arr[i];
    }
    return result;
}

uint32_t Canvas::getStrokeTransformOrder() const TBAG_NOEXCEPT
{
    return _impl ? _impl->context.strokeTransformOrder() : 0;
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
    return _call_blend2d_method(clipToRect, __convert_bl_structure(rect));
}

Err Canvas::clipToRect(Rect2i const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clipToRect, __convert_bl_structure(rect));
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
    return _call_blend2d_method(clearRect, __convert_bl_structure(rect));
}

Err Canvas::clearRect(Rect2i const & rect) TBAG_NOEXCEPT
{
    return _call_blend2d_method(clearRect, __convert_bl_structure(rect));
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
    return _call_blend2d_method(fillPolygon, __bl_cast(poly), n);
}

Err Canvas::fillPolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillPolygon, __bl_cast(poly), n);
}

Err Canvas::fillRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, __bl_cast(data), n);
}

Err Canvas::fillRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(fillRectArray, __bl_cast(data), n);
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
    return _call_blend2d_method(strokePolyline, __bl_cast(poly), n);
}

Err Canvas::strokePolyline(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolyline, __bl_cast(poly), n);
}

Err Canvas::strokePolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, __bl_cast(poly), n);
}

Err Canvas::strokePolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokePolygon, __bl_cast(poly), n);
}

Err Canvas::strokeRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, __bl_cast(data), n);
}

Err Canvas::strokeRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT
{
    return _call_blend2d_method(strokeRectArray, __bl_cast(data), n);
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

