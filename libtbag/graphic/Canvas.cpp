/**
 * @file   Canvas.cpp
 * @brief  Canvas class implementation.
 * @author zer0
 * @date   2020-01-11
 */

#include <libtbag/graphic/Canvas.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <blend2d.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

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

Err Canvas::end() TBAG_NOEXCEPT
{
    if (!_impl) {
        return E_NREADY;
    }
    return convertBLResultToErr(_impl->context.end());
}

Err Canvas::setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT
{
    if (!_impl) {
        return E_NREADY;
    }
    return convertBLResultToErr(_impl->context.setCompOp(__get_blend_mode_to_bl_comp_op(mode)));
}

Err Canvas::setFillStyle(Channel r, Channel g, Channel b, Channel a) TBAG_NOEXCEPT
{
    if (!_impl) {
        return E_NREADY;
    }
    return convertBLResultToErr(_impl->context.setFillStyle(BLRgba32(r, g, b, a)));
}

Err Canvas::setFillStyle(Rgb32 const & color) TBAG_NOEXCEPT
{
    return setFillStyle(color.r, color.g, color.b, color.a);
}

Err Canvas::fillAll() TBAG_NOEXCEPT
{
    if (!_impl) {
        return E_NREADY;
    }
    return convertBLResultToErr(_impl->context.fillAll());
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
            destination[0] = source[0]; // B
            destination[1] = source[1]; // G
            destination[2] = source[2]; // R
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

