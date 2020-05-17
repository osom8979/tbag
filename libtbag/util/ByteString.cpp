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

ErrSize toByteSize(std::string const & str)
{
    return ByteStringParser::parse(str);
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

