/**
 * @file   Color.cpp
 * @brief  Color class prototype.
 * @author zer0
 * @date   2019-06-16
 */

#include <libtbag/graphic/Color.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

Rgb32 convertRgb24ToRgb32(Rgb24 const & src) TBAG_NOEXCEPT
{
    return { src.r, src.g, src.b, channel_max() };
}

Rgb24 convertRgb32ToRgb24(Rgb32 const & src) TBAG_NOEXCEPT
{
    return { src.r, src.g, src.b };
}

std::string convertRgb24ToHexString(Rgb24 const & c)
{
    using namespace libtbag::string;
    return { COLOR_STRING_PREFIX,
             convertHalfByteToHexChar(c.r>>4),
             convertHalfByteToHexChar(c.r),
             convertHalfByteToHexChar(c.g>>4),
             convertHalfByteToHexChar(c.g),
             convertHalfByteToHexChar(c.b>>4),
             convertHalfByteToHexChar(c.b)
    };
}

std::string convertRgb24ToHexString(Rgb32 const & c)
{
    using namespace libtbag::string;
    return { COLOR_STRING_PREFIX,
             convertHalfByteToHexChar(c.r>>4),
             convertHalfByteToHexChar(c.r),
             convertHalfByteToHexChar(c.g>>4),
             convertHalfByteToHexChar(c.g),
             convertHalfByteToHexChar(c.b>>4),
             convertHalfByteToHexChar(c.b)
    };
}

std::string convertRgb32ToHexString(Rgb32 const & c)
{
    using namespace libtbag::string;
    return { COLOR_STRING_PREFIX,
             convertHalfByteToHexChar(c.r>>4),
             convertHalfByteToHexChar(c.r),
             convertHalfByteToHexChar(c.g>>4),
             convertHalfByteToHexChar(c.g),
             convertHalfByteToHexChar(c.b>>4),
             convertHalfByteToHexChar(c.b),
             convertHalfByteToHexChar(c.a>>4),
             convertHalfByteToHexChar(c.a)
    };
}

std::string convertRgb32ToArgbHexString(Rgb32 const & c)
{
    using namespace libtbag::string;
    return { COLOR_STRING_PREFIX,
             convertHalfByteToHexChar(c.a>>4),
             convertHalfByteToHexChar(c.a),
             convertHalfByteToHexChar(c.r>>4),
             convertHalfByteToHexChar(c.r),
             convertHalfByteToHexChar(c.g>>4),
             convertHalfByteToHexChar(c.g),
             convertHalfByteToHexChar(c.b>>4),
             convertHalfByteToHexChar(c.b)
    };
}

Err convertHexStringToRgb32(std::string const & text, Rgb32 & color)
{
    using namespace libtbag::string;
    std::size_t i = 0;
    while (text[i] == COLOR_STRING_PREFIX) {
        ++i;
    }

    auto const COLOR_TEXT_SIZE = text.size() - i;
    if (COLOR_TEXT_SIZE == 3 || COLOR_TEXT_SIZE == 4) {
        Err code;
        uint8_t half_byte;

        code = convertHexCharToHalfByte(text[i+0], half_byte);
        if (isSuccess(code)) {
            color.r = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        code = convertHexCharToHalfByte(text[i+1], half_byte);
        if (isSuccess(code)) {
            color.g = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        code = convertHexCharToHalfByte(text[i+2], half_byte);
        if (isSuccess(code)) {
            color.b = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        if (COLOR_TEXT_SIZE == 4) {
            code = convertHexCharToHalfByte(text[i+3], half_byte);
            if (isSuccess(code)) {
                color.a = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
            } else {
                return code;
            }
        } else {
            color.a = channel_max();
        }
        return E_SUCCESS;

    } else if (COLOR_TEXT_SIZE == 6 || COLOR_TEXT_SIZE == 8) {
        Err code;

        code = convertHexCharToByte(text[i+0], text[i+1], color.r);
        if (isFailure(code)) {
            return code;
        }

        code = convertHexCharToByte(text[i+2], text[i+3], color.g);
        if (isFailure(code)) {
            return code;
        }

        code = convertHexCharToByte(text[i+4], text[i+5], color.b);
        if (isFailure(code)) {
            return code;
        }

        if (COLOR_TEXT_SIZE == 8) {
            code = convertHexCharToByte(text[i+6], text[i+7], color.a);
            if (isFailure(code)) {
                return code;
            }
        } else {
            color.a = channel_max();
        }
        return E_SUCCESS;
    }

    return E_ILLARGS;
}

Err convertArgbHexStringToRgb32(std::string const & text, Rgb32 & color)
{
    using namespace libtbag::string;
    std::size_t i = 0;
    while (text[i] == COLOR_STRING_PREFIX) {
        ++i;
    }

    auto const COLOR_TEXT_SIZE = text.size() - i;
    if (COLOR_TEXT_SIZE == 3 || COLOR_TEXT_SIZE == 4) {
        Err code;
        uint8_t half_byte;

        if (COLOR_TEXT_SIZE == 4) {
            code = convertHexCharToHalfByte(text[i+0], half_byte);
            if (isSuccess(code)) {
                color.a = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
            } else {
                return code;
            }
            ++i;
        } else {
            color.a = channel_max();
        }

        code = convertHexCharToHalfByte(text[i+0], half_byte);
        if (isSuccess(code)) {
            color.r = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        code = convertHexCharToHalfByte(text[i+1], half_byte);
        if (isSuccess(code)) {
            color.g = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        code = convertHexCharToHalfByte(text[i+2], half_byte);
        if (isSuccess(code)) {
            color.b = float_to_channel(static_cast<float>(half_byte) / COLOR_FLOATING_MAX_HALF_BYTE);
        } else {
            return code;
        }

        return E_SUCCESS;

    } else if (COLOR_TEXT_SIZE == 6 || COLOR_TEXT_SIZE == 8) {
        Err code;

        if (COLOR_TEXT_SIZE == 8) {
            code = convertHexCharToByte(text[i+0], text[i+1], color.a);
            if (isFailure(code)) {
                return code;
            }
            i += 2;
        } else {
            color.a = channel_max();
        }

        code = convertHexCharToByte(text[i+0], text[i+1], color.r);
        if (isFailure(code)) {
            return code;
        }

        code = convertHexCharToByte(text[i+2], text[i+3], color.g);
        if (isFailure(code)) {
            return code;
        }

        code = convertHexCharToByte(text[i+4], text[i+5], color.b);
        if (isFailure(code)) {
            return code;
        }
        return E_SUCCESS;
    }

    return E_ILLARGS;
}

Err convertHexStringToRgb24(std::string const & text, Rgb24 & color)
{
    Rgb32 rbg32_color;
    auto const CODE = convertHexStringToRgb32(text, rbg32_color);
    if (isFailure(CODE)) {
        return CODE;
    }
    color = convertRgb32ToRgb24(rbg32_color);
    return E_SUCCESS;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

