/**
 * @file   StringUtils.cpp
 * @brief  String utilities implementation.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: Strings -> StringUtils)
 */

#include <libtbag/string/StringUtils.hpp>
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

