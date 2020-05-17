/**
 * @file   ByteString.cpp
 * @brief  ByteString class implementation.
 * @author zer0
 * @date   2020-05-17
 */

#include <libtbag/util/ByteString.hpp>
#include <libtbag/util/ByteStringParser.hpp.inl>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Unit.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

std::size_t toByteSize(std::string const & str)
{
    using namespace libtbag::string;
    auto const lower_str = trim(lower(str));
    auto const size = lower_str.size();

    std::size_t i = 0;
    char last_c = '\0';

    for (; i < size; ++i) {
        last_c = lower_str[i];
        if (!std::isdigit(last_c)) {
            break;
        }
    }

    // Skip space;
    for (; i < size; ++i) {
        last_c = lower_str[i];
        if (last_c != ' ') {
            break;
        }
    }

    auto const value = toValue<std::size_t>(lower_str.substr(0, i));

    // clang-format off
    switch (last_c) {
    case BYTE_LOWER_CHAR:       return value;
    case KILO_BYTE_LOWER_CHAR:  return value *  KILO_BYTE_TO_BYTE;
    case MEGA_BYTE_LOWER_CHAR:  return value *  MEGA_BYTE_TO_BYTE;
    case GIGA_BYTE_LOWER_CHAR:  return value *  GIGA_BYTE_TO_BYTE;
    case TERA_BYTE_LOWER_CHAR:  return value *  TERA_BYTE_TO_BYTE;
    case PETA_BYTE_LOWER_CHAR:  return value *  PETA_BYTE_TO_BYTE;
    case EXA_BYTE_LOWER_CHAR:   return value *   EXA_BYTE_TO_BYTE;
    case ZETTA_BYTE_LOWER_CHAR: return value * ZETTA_BYTE_TO_BYTE;
    case YOTTA_BYTE_LOWER_CHAR: return value * YOTTA_BYTE_TO_BYTE;
    default:                    return value;
    }
    // clang-format on
}

std::string toUpperByteText(std::size_t byte)
{
#if 0
    // [WARNING] Outbound of 64bit size_t type.
    if (byte >= YOTTA_BYTE_TO_BYTE) {
        return std::to_string(byte/YOTTA_BYTE_TO_BYTE) + YOTTA_BYTE_SHORT_NAME;
    } else if (byte >= ZETTA_BYTE_TO_BYTE) {
        return std::to_string(byte/ZETTA_BYTE_TO_BYTE) + ZETTA_BYTE_SHORT_NAME;
    } else
#endif

    if (byte >= EXA_BYTE_TO_BYTE) {
        return std::to_string(byte/EXA_BYTE_TO_BYTE) + EXA_BYTE_SHORT_NAME;
    } else if (byte >= PETA_BYTE_TO_BYTE) {
        return std::to_string(byte/PETA_BYTE_TO_BYTE) + PETA_BYTE_SHORT_NAME;
    } else if (byte >= TERA_BYTE_TO_BYTE) {
        return std::to_string(byte/TERA_BYTE_TO_BYTE) + TERA_BYTE_SHORT_NAME;
    } else if (byte >= GIGA_BYTE_TO_BYTE) {
        return std::to_string(byte/GIGA_BYTE_TO_BYTE) + GIGA_BYTE_SHORT_NAME;
    } else if (byte >= MEGA_BYTE_TO_BYTE) {
        return std::to_string(byte/MEGA_BYTE_TO_BYTE) + MEGA_BYTE_SHORT_NAME;
    } else if (byte >= KILO_BYTE_TO_BYTE) {
        return std::to_string(byte/KILO_BYTE_TO_BYTE) + KILO_BYTE_SHORT_NAME;
    } else {
        return std::to_string(byte) + BYTE_SHORT_NAME;
    }
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

