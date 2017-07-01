/**
 * @file   StringUtils.cpp
 * @brief  String utilities implementation.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: Strings -> StringUtils)
 */

#include <libtbag/string/StringUtils.hpp>

#include <cctype>
#include <cassert>

#include <algorithm>
#include <random>

#include <unicode/unistr.h>
#include <libtbag/Err.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter)
{
    if (source.empty() || delimiter.empty()) {
        return std::vector<std::string>();
    }

    std::vector<std::string> result;
    std::string token;

    std::size_t start = 0;
    std::size_t end   = source.find(delimiter);

    while (end != std::string::npos) {
        token = source.substr(start, end - start);
        if (token.empty() == false) {
            result.push_back(token);
        }

        // Calculate next token index.
        start = end + delimiter.length();
        end   = source.find(delimiter, start);
    }

    // Last token.
    token = source.substr(start, end);
    if (token.empty() == false) {
        result.push_back(token);
    }

    return result;
}

std::vector<std::string> splitUtf8Tokens(std::string const & utf8_source, std::string const & utf8_delimiter)
{
    if (utf8_source.empty() || utf8_delimiter.empty()) {
        return std::vector<std::string>();
    }

    icu::UnicodeString source    = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_source.c_str()));
    icu::UnicodeString delimiter = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_delimiter.c_str()));

    std::vector<std::string> result;
    icu::UnicodeString token;
    std::string utf8_token;

    int32_t const UNICODE_NPOS = -1;
    int32_t start = 0;
    int32_t end   = source.indexOf(delimiter);

    while (end != UNICODE_NPOS) {
        token = source.tempSubString(start, end - start);
        if (token.isEmpty() == false) {
            utf8_token.clear();
            token.toUTF8String(utf8_token);
            result.push_back(utf8_token);
        }

        // Calculate next token index.
        start = end + delimiter.length();
        end   = source.indexOf(delimiter, start);
    }

    // Last token.
    token = source.tempSubString(start);
    if (token.isEmpty() == false) {
        utf8_token.clear();
        token.toUTF8String(utf8_token);
        result.push_back(utf8_token);
    }

    return result;
}

std::string convertStringWithThreadId(std::thread::id const & id)
{
    std::stringstream ss;
    ss << id;
    return ss.str();
}

char convertHalfByteToHexChar(uint8_t half_byte)
{
    static uint8_t const HALF_BYTE_FILTER = 0x0F;
    switch (half_byte & HALF_BYTE_FILTER) {
    case 0x0: return '0';
    case 0x1: return '1';
    case 0x2: return '2';
    case 0x3: return '3';
    case 0x4: return '4';
    case 0x5: return '5';
    case 0x6: return '6';
    case 0x7: return '7';
    case 0x8: return '8';
    case 0x9: return '9';
    case 0xA: return 'A';
    case 0xB: return 'B';
    case 0xC: return 'C';
    case 0xD: return 'D';
    case 0xE: return 'E';
    case 0xF: return 'F';
    default:  assert(0 && "Inaccessible case."); return '\0';
    }
}

std::string convertByteToHexString(uint8_t hex)
{
    char result[3] = {0,};
    result[0] = convertHalfByteToHexChar(hex >> 4);
    result[1] = convertHalfByteToHexChar(hex);
    result[2] = '\0';
    return std::string(result);
}

std::string convertByteArrayToHexString(std::vector<uint8_t> const & bytes, std::string const & prefix, std::string const & separator)
{
    std::size_t const SIZE = bytes.size();
    if (SIZE == 0) {
        return std::string();
    }

    std::string result;
    std::size_t i = 0;
    while (true) {
        result += (prefix + convertByteToHexString(bytes[i]));
        if ((++i) < SIZE) {
            result += separator;
        } else {
            break;
        }
    }
    return result;
}

Err convertHexCharToHalfByte(char hex_char, uint8_t & result)
{
    switch (hex_char) {
    case '0': result = 0x0; return Err::E_SUCCESS;
    case '1': result = 0x1; return Err::E_SUCCESS;
    case '2': result = 0x2; return Err::E_SUCCESS;
    case '3': result = 0x3; return Err::E_SUCCESS;
    case '4': result = 0x4; return Err::E_SUCCESS;
    case '5': result = 0x5; return Err::E_SUCCESS;
    case '6': result = 0x6; return Err::E_SUCCESS;
    case '7': result = 0x7; return Err::E_SUCCESS;
    case '8': result = 0x8; return Err::E_SUCCESS;
    case '9': result = 0x9; return Err::E_SUCCESS;
    case 'A':
    case 'a': result = 0xA; return Err::E_SUCCESS;
    case 'B':
    case 'b': result = 0xB; return Err::E_SUCCESS;
    case 'C':
    case 'c': result = 0xC; return Err::E_SUCCESS;
    case 'D':
    case 'd': result = 0xD; return Err::E_SUCCESS;
    case 'E':
    case 'e': result = 0xE; return Err::E_SUCCESS;
    case 'F':
    case 'f': result = 0xF; return Err::E_SUCCESS;
    }
    return Err::E_PARSING;
}

Err convertHexStringToByte(char high_char, char low_char, uint8_t & result)
{
    uint8_t high = 0x00;
    uint8_t  low = 0x00;
    if (convertHexCharToHalfByte(high_char, high) != Err::E_SUCCESS) {
        return Err::E_PARSING;
    }
    if (convertHexCharToHalfByte(low_char, low) != Err::E_SUCCESS) {
        return Err::E_PARSING;
    }
    result = (high << 4) | low;
    return Err::E_SUCCESS;
}

TBAG_CONSTEXPR char const TBAG_HEX_ARRAY_BYTES[] = "0123456789ABCDEF";

AddressHexString convertAddressToHexString(void const * address) TBAG_NOEXCEPT
{
    auto addr = reinterpret_cast<std::make_unsigned<std::ptrdiff_t>::type>(address);
    std::size_t const SIZE = sizeof(void*);

    AddressHexString result = {"0x"};
    result.back() = '\0';
    char * rend = result.data() + SIZE * 2 + 1;

    for (std::size_t i = 0; i < SIZE; ++i) {
        *rend = TBAG_HEX_ARRAY_BYTES[(addr & 0xFFu) & 0xF];
        --rend;

        *rend = TBAG_HEX_ARRAY_BYTES[(addr & 0xFFu) >> 4];
        --rend;

        addr >>= 8;
    }
    return result;
}

std::string convertAddressHexStringToString(AddressHexString const & address)
{
    assert(address.back() == '\0');
    return std::string(address.begin(), address.end() - (1/*NULL*/));
}

std::vector<std::string> splitMatch(std::string const & source, std::regex const & match)
{
    using TokenIterator = std::regex_token_iterator<typename std::string::const_iterator>;

    auto itr = TokenIterator(source.begin(), source.end(), match);
    auto end = TokenIterator();

    std::vector<std::string> result;
    while (itr != end) {
        result.push_back(itr->str());
        ++itr;
    }
    return result;
}

std::vector<std::string> splitMatch(std::string const & source, std::string const & match)
{
    return splitMatch(source, std::regex(match));
}

std::string replaceRegex(std::string const & source, std::string const & regex, std::string const & replace)
{
    return std::regex_replace(source, std::regex(regex), replace);
}

std::string removeRegex(std::string const & source, std::string const & regex)
{
    return replaceRegex(source, regex, std::string());
}

bool isMatch(std::string const & original, std::regex const & regex)
{
    return std::regex_match(original, regex);
}

bool isMatch(std::string const & original, std::string const & regex)
{
    return isMatch(original, std::regex(regex));
}

std::string trimLeft(std::string const & str)
{
    auto itr = str.begin();
    for (; itr != str.end(); ++itr) {
        if (*itr != CHAR_SPACE && *itr != CHAR_TAB && *itr != CHAR_RETURN) {
            break;
        }
    }
    return std::string(itr, str.end());
}

std::string trimRight(std::string const & str)
{
    auto itr = str.rbegin();
    for (; itr != str.rend(); ++itr) {
        if (*itr != CHAR_SPACE && *itr != CHAR_TAB && *itr != CHAR_RETURN) {
            break;
        }
    }
    return std::string(str.begin(), itr.base());
}

std::string trim(std::string const & str)
{
    return trimRight(trimLeft(str));
}

bool createRandomString(char * buffer, std::size_t size)
{
    static const char STRING_TEMPLATE[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const std::size_t MIN = 0;
    static const std::size_t MAX = sizeof(STRING_TEMPLATE) / sizeof(STRING_TEMPLATE[0]) - 1/*NULL CHAR*/ - 1/*SIZE TO INDEX*/;

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<std::size_t> distribution(MIN, MAX);

    std::size_t cursor = 0;
    for (std::size_t i = 0; i < size; ++i) {
        buffer[i] = STRING_TEMPLATE[distribution(engine)];
    }

    return true;
}

std::string lower(std::string const & str)
{
    std::string result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), ::tolower);
    return result;
}

std::string upper(std::string const & str)
{
    std::string result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), ::toupper);
    return result;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

