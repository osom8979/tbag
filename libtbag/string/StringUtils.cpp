/**
 * @file   StringUtils.cpp
 * @brief  String utilities implementation.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: Strings -> StringUtils)
 */

#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Unit.hpp>

#include <cctype>
#include <cassert>
#include <cstdio>

#include <iomanip>
#include <algorithm>
#include <random>
#include <sstream>

#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <libtbag/Err.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

LineFeedStyle getLineFeedStyle(std::string const & name)
{
    auto const LOWER = lower(trim(name));
    if (LOWER == "unix" || LOWER == "linux" || LOWER == "lf") {
        return LineFeedStyle::LFS_UNIX;
    }
    if (LOWER == "windows" || LOWER == "window" || LOWER == "win32" || LOWER == "win64" || LOWER == "win" || LOWER == "crlf") {
        return LineFeedStyle::LFS_WINDOWS;
    }
    if (LOWER == "auto" || LOWER == "detect" || LOWER == "platform") {
        return LineFeedStyle::LFS_AUTO;
    }
    return LineFeedStyle::LFS_NONE;
}

char const * getLineFeedStyleText(LineFeedStyle style) TBAG_NOEXCEPT
{
    switch (style) {
    case LineFeedStyle::LFS_NONE:
        return "none";
    case LineFeedStyle::LFS_UNIX:
        return "unix";
    case LineFeedStyle::LFS_WINDOWS:
        return "windows";
    case LineFeedStyle::LFS_AUTO:
        return "auto";
    default:
        return TBAG_EMPTY_STRING;
    }
}

std::string getLineFeed(LineFeedStyle style)
{
    if (style == LineFeedStyle::LFS_AUTO) {
        if (isWindowsPlatform()) {
            return WINDOWS_NEW_LINE;
        } else {
            return UNIX_NEW_LINE;
        }
    } else if (style == LineFeedStyle::LFS_UNIX) {
        return UNIX_NEW_LINE;
    } else if (style == LineFeedStyle::LFS_WINDOWS) {
        return WINDOWS_NEW_LINE;
    } else {
        return {};
    }
}

std::string getStdEndLine()
{
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
}

bool appendLineFeed(LineFeedStyle style, char * buffer, int buffer_size, int offset) TBAG_NOEXCEPT
{
    if (style == LineFeedStyle::LFS_AUTO) {
        style = isWindowsPlatform() ? LineFeedStyle::LFS_WINDOWS : LineFeedStyle::LFS_UNIX;
    }

    if (style == LineFeedStyle::LFS_UNIX) {
        if (offset < buffer_size) {
            buffer[offset] = UNIX_NEW_LINE[0];
            return true;
        } else {
            return false;
        }
    } else if (style == LineFeedStyle::LFS_WINDOWS) {
        if ((offset+1) < buffer_size) {
            buffer[offset+0] = WINDOWS_NEW_LINE[0];
            buffer[offset+1] = WINDOWS_NEW_LINE[1];
            return true;
        } else {
            return false;
        }
    }

    assert(style == LineFeedStyle::LFS_NONE);
    return true;
}

std::vector<std::string> splitTokens(std::string const & source, std::string const & delimiter, bool remove_empty)
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
        if (remove_empty == false || token.empty() == false) {
            result.push_back(token);
        }

        // Calculate next token index.
        start = end + delimiter.length();
        end   = source.find(delimiter, start);
    }

    // Last token.
    token = source.substr(start, end);
    if (remove_empty == false || token.empty() == false) {
        result.push_back(token);
    }

    return result;
}

std::vector<std::string> splitUtf8Tokens(std::string const & utf8_source, std::string const & utf8_delimiter, bool remove_empty)
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
        if (remove_empty == false || token.isEmpty() == false) {
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
    if (remove_empty == false || token.isEmpty() == false) {
        utf8_token.clear();
        token.toUTF8String(utf8_token);
        result.push_back(utf8_token);
    }

    return result;
}

std::vector<std::string> splitSize(std::string const & source, std::size_t size)
{
    assert(size >= 1u);
    if (source.empty()) {
        return {};
    }

    auto const * data = source.c_str();
    assert(data != nullptr);

    std::vector<std::string> result;
    auto remain_size = source.size();
    auto length = remain_size <= size ? remain_size : size;

    do {
        result.emplace_back(std::string(data, data + length));
        data += length;
        remain_size -= length;
        length = remain_size <= size ? remain_size : size;
    } while (remain_size >= 1);

    return result;
}

std::pair<std::string, std::string> divideTwo(std::string const & source, std::string const & delimiter)
{
    auto const end = source.find(delimiter);
    if (end == std::string::npos) {
        return {source, STRING_EMPTY};
    }
    return {source.substr(0, end), source.substr(end+delimiter.size())};
}

Err splitNumberAndUnit(std::string const & str,
                       std::string & number,
                       std::string & unit)
{
    auto const lower_str = lower(trim(str));
    auto const size = lower_str.size();

    std::stringstream number_ss;
    std::size_t i = 0;
    bool is_floating = false;
    char c;

    for (; i < size; ++i) {
        c = lower_str[i];
        if (std::isdigit(c)) {
            number_ss << lower_str[i];
        } else if (c == ',') {
            // Skip.
        } else if (c == '.') {
            if (is_floating) {
                return E_PARSING;
            } else {
                is_floating = true;
                number_ss.put('.');
            }
        } else {
            break;
        }
    }

    // Skip space;
    for (; i < size; ++i) {
        if (!isWhiteSpace(lower_str[i])) {
            break;
        }
    }

    number = number_ss.str();
    unit = lower_str.substr(i);
    return E_SUCCESS;
}

std::string mergeTokens(std::vector<std::string> const & tokens, std::string const & delimiter)
{
    return mergeTokens(tokens.begin(), tokens.end(), delimiter);
}

std::string mergeTokens(std::vector<std::string> const & tokens)
{
    return mergeTokens(tokens, STRING_SPACE);
}

std::string mergeArgv(char ** argv)
{
    std::string result;
    while (argv != nullptr && (*argv) != nullptr) {
        result += (*argv);
        ++argv;
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

std::string convertByteArrayToHexString(
        uint8_t const * bytes,
        std::size_t size,
        std::string const & prefix,
        std::string const & separator)
{
    if (size == 0) {
        return std::string();
    }

    std::string result;
    std::size_t i = 0;
    while (true) {
        result += (prefix + convertByteToHexString(bytes[i]));
        if ((++i) < size) {
            result += separator;
        } else {
            break;
        }
    }
    return result;
}

std::string convertByteArrayToHexStringBox(
        uint8_t const * bytes,
        std::size_t size,
        int line_width,
        std::string const & prefix,
        std::string const & separator,
        std::string const & new_line)
{
    if (line_width < 1) {
        line_width = DEFAULT_LINE_WIDTH;
    }
    if (line_width >= size) {
        return convertByteArrayToHexString(bytes, size, prefix, separator);
    }

    std::stringstream ss;
    ss << convertByteArrayToHexString(bytes, line_width, prefix, separator);
    bytes += line_width;
    size = size > line_width ? size - line_width : 0;

    std::size_t data_size;
    while (size > 0) {
        ss << new_line << convertByteArrayToHexString(bytes, (line_width < size ? line_width : size), prefix, separator);
        bytes += line_width;
        size = size > line_width ? size - line_width : 0;
    }

    return ss.str();
}

Err convertHexCharToHalfByte(char hex_char, uint8_t & result)
{
    switch (hex_char) {
    case '0': result = 0x0; return E_SUCCESS;
    case '1': result = 0x1; return E_SUCCESS;
    case '2': result = 0x2; return E_SUCCESS;
    case '3': result = 0x3; return E_SUCCESS;
    case '4': result = 0x4; return E_SUCCESS;
    case '5': result = 0x5; return E_SUCCESS;
    case '6': result = 0x6; return E_SUCCESS;
    case '7': result = 0x7; return E_SUCCESS;
    case '8': result = 0x8; return E_SUCCESS;
    case '9': result = 0x9; return E_SUCCESS;
    case 'A': TBAG_FALLTHROUGH
    case 'a': result = 0xA; return E_SUCCESS;
    case 'B': TBAG_FALLTHROUGH
    case 'b': result = 0xB; return E_SUCCESS;
    case 'C': TBAG_FALLTHROUGH
    case 'c': result = 0xC; return E_SUCCESS;
    case 'D': TBAG_FALLTHROUGH
    case 'd': result = 0xD; return E_SUCCESS;
    case 'E': TBAG_FALLTHROUGH
    case 'e': result = 0xE; return E_SUCCESS;
    case 'F': TBAG_FALLTHROUGH
    case 'f': result = 0xF; return E_SUCCESS;
    }
    return E_PARSING;
}

Err convertHexCharToByte(char high_char, char low_char, uint8_t & result)
{
    uint8_t high = 0x00;
    uint8_t  low = 0x00;
    if (convertHexCharToHalfByte(high_char, high) != E_SUCCESS) {
        return E_PARSING;
    }
    if (convertHexCharToHalfByte(low_char, low) != E_SUCCESS) {
        return E_PARSING;
    }
    result = (high << 4) | low;
    return E_SUCCESS;
}

Err convertHexStringToBuffer(char const * hex_string, std::size_t length, libtbag::util::Buffer & buffer)
{
    assert((length & 0x1) == 0x0); // Odd length strings are not supported.
    assert(hex_string != nullptr);
    assert(length >= 1);

    auto const BUFFER_SIZE = (length / 2);
    buffer.resize(BUFFER_SIZE);

    Err code;
    uint8_t byte;
    for (std::size_t i = 0; i < length; i += 2) {
        code = convertHexCharToByte(hex_string[i], hex_string[i+1], byte);
        if (isFailure(code)) {
            return code;
        }
        buffer[i/2] = (libtbag::util::Buffer::value_type)byte;
    }
    return E_SUCCESS;
}

AddressHexString convertAddressToHexString(void const * address) TBAG_NOEXCEPT
{
    return convertUnsignedIntegerToHexString((std::uint64_t)address);
}

TBAG_CONSTEXPR static char const TBAG_HEX_ARRAY_BYTES[] = "0123456789ABCDEF";

AddressHexString convertUnsignedIntegerToHexString(std::uint64_t value) TBAG_NOEXCEPT
{
    AddressHexString result = {"0x"};
    result.back() = '\0';
    char * rend = result.data() + sizeof(std::uint64_t) * 2 + 1;

    for (std::size_t i = 0; i < sizeof(std::uint64_t); ++i) {
        *rend = TBAG_HEX_ARRAY_BYTES[(value & 0xFFu) & 0xF];
        --rend;

        *rend = TBAG_HEX_ARRAY_BYTES[(value & 0xFFu) >> 4];
        --rend;

        value >>= 8;
    }
    return result;
}

std::string convertAddressHexStringToString(AddressHexString const & address)
{
    assert(address.back() == '\0');
    return std::string(address.begin(), address.end() - (1/*NULL*/));
}

std::string convertAddressToString(void const * address)
{
    return convertAddressHexStringToString(convertAddressToHexString(address));
}

std::string convertByteArrayToPrettyHexStringLine(
        uint8_t const * bytes,
        std::size_t size,
        int line_width,
        bool print_address,
        bool print_ascii)
{
    std::stringstream ss;
    if (print_address) {
        ss << convertAddressToString(bytes);
        ss.put('|');
    }

    if (print_ascii) {
        for (auto i = 0; i < line_width; ++i) {
            if (i < size) {
                auto const c = bytes[i];
                if (std::isprint(c)) {
                    ss.put(c);
                } else {
                    ss.put(CHAR_SPACE);
                }
            } else {
                ss.put(CHAR_SPACE);
            }
        }
        ss.put('|');
    }

    ss << convertByteArrayToHexStringBox(bytes, size, 2, STRING_EMPTY, STRING_EMPTY, STRING_SPACE);

    return ss.str();
}

std::string convertByteArrayToPrettyHexStringBox(
        uint8_t const * bytes,
        std::size_t size,
        int line_width,
        bool print_address,
        bool print_ascii)
{
    if (line_width < 1) {
        line_width = DEFAULT_LINE_WIDTH;
    }
    if (line_width >= size) {
        return convertByteArrayToPrettyHexStringLine(bytes, size, line_width, print_address, print_ascii);
    }

    std::stringstream ss;
    ss << convertByteArrayToPrettyHexStringLine(bytes, line_width, line_width, print_address, print_ascii);
    bytes += line_width;
    size = size > line_width ? size - line_width : 0;

    std::size_t data_size;
    while (size > 0) {
        auto const current_byte_size = (line_width < size ? line_width : size);
        ss << NEW_LINE << convertByteArrayToPrettyHexStringLine(bytes, current_byte_size, line_width,
                                                                print_address, print_ascii);
        bytes += line_width;
        size = size > line_width ? size - line_width : 0;
    }

    return ss.str();
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

std::string replace(std::string const & source, std::string const & find, std::string const & replace)
{
    auto const pos = source.find(find);
    if (pos != std::string::npos) {
        std::string result = source;
        result.replace(pos, find.size(), replace);
        return result;
    }
    return source;
}

bool isMatch(std::string const & source, std::regex const & regex)
{
    return std::regex_match(source, regex);
}

bool isMatch(std::string const & source, std::string const & regex)
{
    return isMatch(source, std::regex(regex));
}

bool isUtf8Match(std::string const & utf8_source, std::string const & regex)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::RegexMatcher matcher(icu::UnicodeString::fromUTF8(icu::StringPiece(regex.c_str())), 0, status);
    if (U_FAILURE(status)) {
        // Handle any syntax errors in the regular expression here
        return false;
    }

    matcher.reset(icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_source.c_str())));
    if (matcher.find() == TRUE) {
        int const START_OF_MATCH = matcher.start(status); // string index of start of match.
        return true;
    }
    return false;
}

bool isDigit(std::string const & str)
{
    if (str.empty()) {
        return false;
    }
    auto const end = str.end();
    for (auto itr = str.begin(); itr != end; ++itr) {
        if (!std::isdigit(*itr)) {
            return false;
        }
    }
    return true;
}

bool isVariableName(char c) TBAG_NOEXCEPT
{
    // clang-format off
    switch (c) {
    case '_':
    case 'A': case 'a':
    case 'B': case 'b':
    case 'C': case 'c':
    case 'D': case 'd':
    case 'E': case 'e':
    case 'F': case 'f':
    case 'G': case 'g':
    case 'H': case 'h':
    case 'I': case 'i':
    case 'J': case 'j':
    case 'K': case 'k':
    case 'L': case 'l':
    case 'M': case 'm':
    case 'N': case 'n':
    case 'O': case 'o':
    case 'P': case 'p':
    case 'Q': case 'q':
    case 'R': case 'r':
    case 'S': case 's':
    case 'T': case 't':
    case 'U': case 'u':
    case 'V': case 'v':
    case 'W': case 'w':
    case 'X': case 'x':
    case 'Y': case 'y':
    case 'Z': case 'z':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return true;
    default:
        return false;
    }
    // clang-format on
}

bool isVariableName(std::string const & str)
{
    if (str.empty()) {
        return false;
    }
    auto const end = str.end();
    for (auto itr = str.begin(); itr != end; ++itr) {
        if (!isVariableName(*itr)) {
            return false;
        }
    }
    return true;
}

bool isWhiteSpace(char c) TBAG_NOEXCEPT
{
    switch (c) {
    case CHAR_NULL:
    case CHAR_SPACE:
    case CHAR_TAB:
    case CHAR_RETURN:
    case CHAR_CARRIAGE_RETURN:
        return true;
    default:
        return false;
    }
}

std::string trimLeft(std::string const & str, std::string const & trim_list)
{
    auto itr = str.begin();
    for (; itr != str.end(); ++itr) {
        if (std::find(trim_list.cbegin(), trim_list.cend(), *itr) == trim_list.cend()) {
            break;
        }
    }
    return std::string(itr, str.end());
}

std::string trimRight(std::string const & str, std::string const & trim_list)
{
    auto itr = str.rbegin();
    for (; itr != str.rend(); ++itr) {
        if (std::find(trim_list.cbegin(), trim_list.cend(), *itr) == trim_list.cend()) {
            break;
        }
    }
    return std::string(str.begin(), itr.base());
}

std::string trim(std::string const & str, std::string const & trim_list)
{
    return trimRight(trimLeft(str, trim_list), trim_list);
}

std::string trimLeft(std::string const & str, char trim_value)
{
    auto itr = str.begin();
    for (; itr != str.end(); ++itr) {
        if (*itr != trim_value) {
            break;
        }
    }
    return std::string(itr, str.end());
}

std::string trimRight(std::string const & str, char trim_value)
{
    auto itr = str.rbegin();
    for (; itr != str.rend(); ++itr) {
        if (*itr != trim_value) {
            break;
        }
    }
    return std::string(str.begin(), itr.base());
}

std::string trim(std::string const & str, char trim_value)
{
    return trimRight(trimLeft(str, trim_value), trim_value);
}

std::string trimLeft(std::string const & str)
{
    auto itr = str.begin();
    for (; itr != str.end(); ++itr) {
        if (!isWhiteSpace(*itr)) {
            break;
        }
    }
    return std::string(itr, str.end());
}

std::string trimRight(std::string const & str)
{
    auto itr = str.rbegin();
    for (; itr != str.rend(); ++itr) {
        if (!isWhiteSpace(*itr)) {
            break;
        }
    }
    return std::string(str.begin(), itr.base());
}

std::string trim(std::string const & str)
{
    return trimRight(trimLeft(str));
}

std::vector<std::string> trim(std::vector<std::string> const & strs, bool remove_empty)
{
    if (remove_empty) {
        std::vector<std::string> result;
        for (auto const & str : strs) {
            auto const trimmed_str = trim(str);
            if (!trimmed_str.empty()) {
                result.emplace_back(trimmed_str);
            }
        }
        return result;
    } else {
        auto const size = strs.size();
        std::vector<std::string> result(size);
        for (auto i = 0; i < size; ++i) {
            result[i] = trim(strs[i]);
        }
        return result;
    }
}

void createRandomString(char * buffer, std::size_t size, char const * table,
                        std::size_t min, std::size_t max)
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<std::size_t> distribution(min, max);
    for (auto i = 0; i < size; ++i) {
        buffer[i] = table[distribution(engine)];
    }
}

void createRandomString(char * buffer, std::size_t size)
{
    static const char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const std::size_t min = 0;
    static const std::size_t max = sizeof(table) / sizeof(table[0]) - 1/*NULL CHAR*/ - 1/*SIZE TO INDEX*/;
    createRandomString(buffer, size, table, min, max);
}

std::string createRandomString(std::size_t size)
{
    std::string result(CHAR_SPACE, size);
    createRandomString(&result[0], size);
    return result;
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

void lowerSelf(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void upperSelf(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

#ifndef _TBAG_STRING_TO_INTEGER_IMPLEMENT
#define _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, func, index, base) \
    try { to = func(str, index, base); return true; } catch (...) { return false; }
#endif

#ifndef _TBAG_STRING_TO_FLOATING_IMPLEMENT
#define _TBAG_STRING_TO_FLOATING_IMPLEMENT(str, to, func, index) \
    try { to = func(str, index); return true; } catch (...) { return false; }
#endif

bool toVal(std::string const & str, bool & to, std::size_t * index, int base)
{
    // clang-format off
    auto const TRIMMED_LOWER_STRING = lower(trim(str));
    if (TRIMMED_LOWER_STRING ==  TRUE_LOWER_STR1) { to =  true; return true; }
    if (TRIMMED_LOWER_STRING ==  TRUE_LOWER_STR2) { to =  true; return true; }
    if (TRIMMED_LOWER_STRING ==  TRUE_LOWER_STR3) { to =  true; return true; }
    if (TRIMMED_LOWER_STRING ==  TRUE_LOWER_STR4) { to =  true; return true; }
    if (TRIMMED_LOWER_STRING ==  TRUE_LOWER_STR5) { to =  true; return true; }
    if (TRIMMED_LOWER_STRING == FALSE_LOWER_STR1) { to = false; return true; }
    if (TRIMMED_LOWER_STRING == FALSE_LOWER_STR2) { to = false; return true; }
    if (TRIMMED_LOWER_STRING == FALSE_LOWER_STR3) { to = false; return true; }
    if (TRIMMED_LOWER_STRING == FALSE_LOWER_STR4) { to = false; return true; }
    // clang-format on

    int temp = 0;
    bool const RESULT = toVal(str, temp, index, base);
    if (RESULT) {
        to = (temp ? true : false);
    }
    return RESULT;
}

// clang-format off
bool toVal(std::string const & str,               char & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stol  , index, base); }
bool toVal(std::string const & str, unsigned      char & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoul , index, base); }
bool toVal(std::string const & str,              short & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoi  , index, base); }
bool toVal(std::string const & str, unsigned     short & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoul , index, base); }
bool toVal(std::string const & str,                int & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoi  , index, base); }
bool toVal(std::string const & str, unsigned       int & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoul , index, base); }
bool toVal(std::string const & str,               long & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stol  , index, base); }
bool toVal(std::string const & str, unsigned      long & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoul , index, base); }
bool toVal(std::string const & str,          long long & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoll , index, base); }
bool toVal(std::string const & str, unsigned long long & to, std::size_t * index, int base) { _TBAG_STRING_TO_INTEGER_IMPLEMENT(str, to, std::stoull, index, base); }

bool toVal(std::string const & str, float & to, std::size_t * index, int UNUSED_PARAM(base))
{ _TBAG_STRING_TO_FLOATING_IMPLEMENT(str, to, std::stof, index); }
bool toVal(std::string const & str, double & to, std::size_t * index, int UNUSED_PARAM(base))
{ _TBAG_STRING_TO_FLOATING_IMPLEMENT(str, to, std::stod, index); }
bool toVal(std::string const & str, long double & to, std::size_t * index, int UNUSED_PARAM(base))
{ _TBAG_STRING_TO_FLOATING_IMPLEMENT(str, to, std::stold, index); }
// clang-format on

bool toVal(std::string const & str, std::string & to, std::size_t * UNUSED_PARAM(index), int UNUSED_PARAM(base))
{
    to = str;
    return true;
}

std::string trimRightOfRealNumberText(std::string const & real_number_text)
{
    auto itr = std::find(real_number_text.begin(), real_number_text.end(), '.');
    if (itr == real_number_text.end()) {
        // Not found dot('.') character.
        // In this case it can be an integer real_number_text.
        return real_number_text;
    }

    assert(*itr == '.');

    ++itr;
    if (itr == real_number_text.end()) {
        return real_number_text + "0"; // e.g. "0." -> "0.0"
    }

    assert(std::isdigit(*itr));

    auto ritr = real_number_text.rbegin();
    for (; (ritr+1).base() != itr; ++ritr) {
        if (*ritr != '0') {
            break;
        }
    }
    return std::string(real_number_text.begin(), ritr.base());
}

template <typename T>
static std::string __to_hex_string(char const * format, T val)
{
    AddressHexString buffer;
    auto const length = ::snprintf(buffer.data(), buffer.size(), format, val);
    assert(length >= 1);
    return std::string(buffer.data(), buffer.data() + length);
}

std::string toHexString(unsigned char val, bool upper)
{
    return __to_hex_string(upper ? "%hhX" : "%hhx", val);
}

std::string toHexString(unsigned short val, bool upper)
{
    return __to_hex_string(upper ? "%hX" : "%hx", val);
}

std::string toHexString(unsigned int val, bool upper)
{
    return __to_hex_string(upper ? "%X" : "%x", val);
}

std::string toHexString(unsigned long val, bool upper)
{
    return __to_hex_string(upper ? "%lX" : "%lx", val);
}

std::string toHexString(unsigned long long val, bool upper)
{
    return __to_hex_string(upper ? "%llX" : "%llx", val);
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

