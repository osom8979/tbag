/**
 * @file   PercentEncoding.cpp
 * @brief  PercentEncoding class implementation.
 * @author zer0
 * @date   2018-11-22
 */

#include <libtbag/codec/PercentEncoding.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <sstream>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace codec {

TBAG_CONSTEXPR static char const PERCENTAGE_CHAR = '%';

bool isReservedCharacters(char c) TBAG_NOEXCEPT
{
    switch (c) {
    case '!': case '*': case '\'':
    case '(': case ')': case ';': case ':':
    case '@': case '&': case '=': case '+':
    case '$': case ',': case '/': case '?':
    case '#': case '[': case ']':
        return true;
    default:
        return false;
    }
}

bool isUnreservedCharacters(char c) TBAG_NOEXCEPT
{
    // Lowercase is the most used character, so compare first.
    if ('a' <= COMPARE_AND(c) <= 'z') {
        return true;
    }

    // Second, Uppercase character.
    if ('A' <= COMPARE_AND(c) <= 'Z') {
        return true;
    }

    // Third, Digit character.
    if ('0' <= COMPARE_AND(c) <= '9') {
        return true;
    }

    // Last, Special character.
    return (c == '-' || c == '_' || c == '.' || c == '~');
}

bool isPlainCharacters(char c) TBAG_NOEXCEPT
{
    return isUnreservedCharacters(c) || isReservedCharacters(c);
}

bool isEncodeCharacters(char c) TBAG_NOEXCEPT
{
    return !isPlainCharacters(c);
}

std::string encodePercent(std::string const & text)
{
    if (text.empty()) {
        return std::string();
    }

    std::size_t const SIZE = text.size();
    std::stringstream ss;

    for (std::size_t i = 0; i < SIZE; ++i) {
        if (isEncodeCharacters(text[i])) {
            using namespace libtbag::string;
            ss << PERCENTAGE_CHAR
               << convertHalfByteToHexChar((uint8_t)text[i] >> 4)
               << convertHalfByteToHexChar((uint8_t)text[i]);
        } else {
            ss << text[i];
        }
    }

    return ss.str();
}

enum decode_percentage_tag
{
    decode_percentage_none,
    decode_percentage_begin_1,
    decode_percentage_begin_2,
};

std::string decodePercent(std::string const & text)
{
    if (text.empty()) {
        return std::string();
    }

    std::size_t const SIZE = text.size();
    std::stringstream ss;

    decode_percentage_tag tag = decode_percentage_none;

    char       c = 0;
    char  high_c = 0;
    uint8_t high = 0;
    uint8_t  low = 0;

    for (std::size_t i = 0; i < SIZE; ++i) {
        using namespace libtbag::string;
        c = text[i];

        switch (tag) {
        case decode_percentage_none:
            if (c == PERCENTAGE_CHAR) {
                tag = decode_percentage_begin_1;
            } else {
                ss << c;
            }
            break;

        case decode_percentage_begin_1:
            if (isSuccess(convertHexCharToHalfByte(c, high))) {
                high_c = c;
                tag = decode_percentage_begin_2;
            } else {
                ss << PERCENTAGE_CHAR << c;
                tag = decode_percentage_none;
            }
            break;

        case decode_percentage_begin_2:
            if (isSuccess(convertHexCharToHalfByte(c, low))) {
                ss << static_cast<char>((high << 4) | low);
            } else {
                ss << PERCENTAGE_CHAR << high_c << c;
            }
            tag = decode_percentage_none;
            break;

        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
        }
    }

    // Flush the remaining data.
    if (tag == decode_percentage_begin_1) {
        ss << PERCENTAGE_CHAR;
    } else if (tag == decode_percentage_begin_2) {
        ss << PERCENTAGE_CHAR << high_c;
    }

    return ss.str();
}

} // namespace codec

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

