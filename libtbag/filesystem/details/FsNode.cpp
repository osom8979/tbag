/**
 * @file   FsNode.cpp
 * @brief  FsNode class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsNode.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <unicode/unistr.h>
#include <unicode/uchar.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// ---------------
namespace __impl {
// ---------------

template <typename Predicated>
void removeLast(icu::UnicodeString & unicode, Predicated checker, bool check_result = true)
{
    int32_t const LENGTH = unicode.length();
    int32_t remove_last_of = 0;
    for (int32_t i = 0; i < LENGTH; ++i) {
        if (checker(unicode.charAt(LENGTH - i - 1)) == check_result) {
            ++remove_last_of;
        } else {
            break;
        }
    }
    unicode.truncate(LENGTH - remove_last_of);
}

template <typename PathSeparator>
std::string removeLastSeparatorWithUtf8(std::string const & utf8_path, PathSeparator checker)
{
    if (utf8_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    if (PATH_LENGTH <= 0) {
        assert(PATH_LENGTH == 0);
        return std::string();
    }

    removeLast(path, checker);

    std::string result;
    path.toUTF8String(result);
    return result;
}

template <typename PathSeparator>
std::string removeDuplicateSeparators(std::string const & utf8_path, UChar separator, PathSeparator checker)
{
    if (utf8_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    if (PATH_LENGTH <= 0) {
        assert(PATH_LENGTH == 0);
        return std::string();
    }

    icu::UnicodeString buffer;
    UChar cursor;
    for (int32_t i = 0; i < PATH_LENGTH; ++i) {
        cursor = path.charAt(i);
        if (checker(cursor)) {
            if (checker(buffer.charAt(buffer.length() - 1)) == false) {
                buffer.append(separator);
            }
        } else {
            buffer.append(cursor);
        }
    }

    std::string result;
    buffer.toUTF8String(result);
    return result;
}

template <typename PathSeparator>
std::string removeLastNodeWithUtf8(std::string const & utf8_path, PathSeparator checker)
{
    if (utf8_path.empty()) {
        return std::string();
    }

    // @formatter:off
    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    if (path.length() <= 0) { return std::string(); }
    removeLast(path, checker,  true); // REMOVE LAST PATH_SEPARATOR.
    if (path.length() <= 0) { return std::string(); }
    removeLast(path, checker, false); // REMOVE LAST NODE.
    if (path.length() <= 0) { return std::string(); }
    //removeLast(path, checker,  true); // REMOVE LAST PATH_SEPARATOR.
    //if (path.length() <= 0) { return std::string(); }
    // @formatter:on

    std::string result;
    path.toUTF8String(result);
    return result;
}

// ------------------
} // namespace __impl
// ------------------

// -----------
namespace uv {
// -----------

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{
    return __impl::removeLastSeparatorWithUtf8(utf8_path, uv::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_POSIX, uv::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, uv::isPathSeparatorChar<UChar>);
}

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{
    return __impl::removeLastNodeWithUtf8(utf8_path, uv::isPathSeparatorChar<UChar>);
}

std::string getNativeWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path));
}

std::string getGenericWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path));
}

std::string getRootDirWithUtf8(std::string const & utf8_path)
{
    if (utf8_path.size() < 1 || utf8_path[0] != PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return std::string() + PATH_SEPARATOR_OF_POSIX;
}

bool isAbsoluteWithUtf8(std::string const & utf8_path)
{
    return !getRootDirWithUtf8(utf8_path).empty();
}

bool isRelativeWithUtf8(std::string const & utf8_path)
{
    return !isAbsoluteWithUtf8(utf8_path);
}

std::string appendParentWithUtf8(std::string const & utf8_path)
{
    return utf8_path + PATH_SEPARATOR_OF_POSIX + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_POSIX;
    std::string const ROOT = getRootDirWithUtf8(utf8_path);

    std::vector<std::string> result;
    result = string::splitUtf8Tokens(removeDuplicateSeparatorsWithUtf8(utf8_path), UTF8_DELIMITER);

    if (!ROOT.empty()) {
        // Force insert the POSIX root directory.
        result.insert(result.begin(), ROOT);
    }
    return result;
}

// --------------
} // namespace uv
// --------------

// ----------------
namespace windows {
// ----------------

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{
    return __impl::removeLastSeparatorWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_WINDOWS, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{
    return __impl::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, windows::isPathSeparatorChar<UChar>);
}

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{
    return __impl::removeLastNodeWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>);
}

std::string getNativeWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path));
}

std::string getGenericWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path));
}

std::string getRootDirWithUtf8(std::string const & utf8_path)
{
    if (utf8_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    if (path.length() < 2 || u_isalpha(path.charAt(0)) == false || path.charAt(1) != ':') {
        return std::string();
    }

    icu::UnicodeString buffer = path.tempSubString(0, 2);
    assert(buffer.length() == 2);

    std::string result;
    buffer.toUTF8String(result);
    return result;
}

bool isAbsoluteWithUtf8(std::string const & utf8_path)
{
    return !getRootDirWithUtf8(utf8_path).empty();
}

bool isRelativeWithUtf8(std::string const & utf8_path)
{
    return !isAbsoluteWithUtf8(utf8_path);
}

std::string appendParentWithUtf8(std::string const & utf8_path)
{
    return utf8_path + PATH_SEPARATOR_OF_WINDOWS + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_WINDOWS;
    return string::splitUtf8Tokens(removeDuplicateSeparatorsWithUtf8(utf8_path), UTF8_DELIMITER);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

