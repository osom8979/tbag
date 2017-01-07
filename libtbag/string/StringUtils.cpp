/**
 * @file   StringUtils.cpp
 * @brief  String utilities implementation.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: Strings -> StringUtils)
 */

#include <libtbag/string/StringUtils.hpp>
#include <cassert>
#include <random>
#include <unicode/unistr.h>

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
    result[0] = convertHalfByteToHexChar(hex >> 1);
    result[1] = convertHalfByteToHexChar(hex);
    result[2] = '\0';
    return std::string(result);
}

std::string convertByteArrayToHexString(std::vector<uint8_t> const & bytes, std::string const & prefix)
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
            result += ' ';
        } else {
            break;
        }
    }
    return result;
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

std::string replaceRegex(std::string const & path, std::string const & regex, std::string const & replace)
{
    return std::regex_replace(path, std::regex(regex), replace);
}

std::string removeRegex(std::string const & path, std::string const & regex)
{
    return replaceRegex(path, regex, std::string());
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

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

