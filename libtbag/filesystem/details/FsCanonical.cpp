/**
 * @file   FsCanonical.cpp
 * @brief  FsCanonical class implementation.
 * @author zer0
 * @date   2017-03-16
 */

#include <libtbag/filesystem/details/FsCanonical.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

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
static void removeLast(icu::UnicodeString & unicode, Predicated checker, bool check_result = true)
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
static std::string removeLastSeparator(std::string const & original_path, PathSeparator checker)
{
    if (original_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(original_path.c_str()));
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
static std::string removeDuplicateSeparators(std::string const & original_path, UChar separator, PathSeparator checker)
{
    if (original_path.empty()) {
        return std::string();
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(original_path.c_str()));
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
static std::string removeLastNode(std::string const & original_path, PathSeparator checker)
{
    if (original_path.empty()) {
        return std::string();
    }

    // @formatter:off
    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(original_path.c_str()));
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

// -------------
namespace unix {
// -------------

std::string removeLastSeparator(std::string const & path)
{
    return __impl::removeLastSeparator(path, unix::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparators(std::string const & path)
{
    return __impl::removeDuplicateSeparators(path, PATH_SEPARATOR_OF_POSIX, unix::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGeneric(std::string const & path)
{
    return __impl::removeDuplicateSeparators(path, PATH_SEPARATOR_OF_GENERIC, unix::isPathSeparatorChar<UChar>);
}

std::string removeLastNode(std::string const & path)
{
    return __impl::removeLastNode(path, unix::isPathSeparatorChar<UChar>);
}

// ----------------
} // namespace unix
// ----------------

// ----------------
namespace windows {
// ----------------

std::string removeLastSeparator(std::string const & path)
{
    return __impl::removeLastSeparator(path, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparators(std::string const & path)
{
    return __impl::removeDuplicateSeparators(path, PATH_SEPARATOR_OF_WINDOWS, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGeneric(std::string const & path)
{
    return __impl::removeDuplicateSeparators(path, PATH_SEPARATOR_OF_GENERIC, windows::isPathSeparatorChar<UChar>);
}

std::string removeLastNode(std::string const & path)
{
    return __impl::removeLastNode(path, windows::isPathSeparatorChar<UChar>);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

