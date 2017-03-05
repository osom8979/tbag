/**
 * @file   FsProhibited.cpp
 * @brief  FsProhibited class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsProhibited.hpp>
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

template <typename Prohibited>
bool isProhibitedNameWithUtf8(std::string const & utf8_path, Prohibited checker)
{
    if (utf8_path.empty()) {
        return true;
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    for (int32_t i = 0; i < PATH_LENGTH; ++i) {
        if (checker(path.charAt(i))) {
            return true;
        }
    }
    return false;
}

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

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return __impl::isProhibitedNameWithUtf8(utf8_path, uv::isProhibitedChar<UChar>);
}

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


// --------------
} // namespace uv
// --------------

// ----------------
namespace windows {
// ----------------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return __impl::isProhibitedNameWithUtf8(utf8_path, windows::isProhibitedChar<UChar>);
}

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

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

