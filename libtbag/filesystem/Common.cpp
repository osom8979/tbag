/**
 * @file   Common.cpp
 * @brief  Common filesystem methods.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/filesystem/Common.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <cassert>

#include <unicode/unistr.h>
#include <unicode/uchar.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

// ================
namespace details {

template <typename Prohibited>
bool isProhibitedNameWithUtf8(std::string const utf8_path, Prohibited checker)
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

// @formatter:off
namespace windows {

bool isProhibitedNameWithUtf8(std::string const utf8_path)
{ return details::isProhibitedNameWithUtf8(utf8_path, windows::isProhibitedChar<UChar>); }

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{ return details::removeLastSeparatorWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>); }

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{ return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_WINDOWS, windows::isPathSeparatorChar<UChar>); }

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{ return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, windows::isPathSeparatorChar<UChar>); }

std::string getNativeWithUtf8(std::string const & utf8_path)
{ return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path)); }

std::string getGenericWithUtf8(std::string const & utf8_path)
{ return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path)); }

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
{ return !getRootDirWithUtf8(utf8_path).empty(); }

bool isRelativeWithUtf8(std::string const & utf8_path)
{ return !isAbsoluteWithUtf8(utf8_path); }

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{ return details::removeLastNodeWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>); }

} // namespace windows
namespace unix {

bool isProhibitedNameWithUtf8(std::string const utf8_path)
{ return details::isProhibitedNameWithUtf8(utf8_path, unix::isProhibitedChar<UChar>); }

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{ return details::removeLastSeparatorWithUtf8(utf8_path, unix::isPathSeparatorChar<UChar>); }

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{ return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_POSIX, unix::isPathSeparatorChar<UChar>); }

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{ return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, unix::isPathSeparatorChar<UChar>); }

std::string getNativeWithUtf8(std::string const & utf8_path)
{ return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path)); }

std::string getGenericWithUtf8(std::string const & utf8_path)
{ return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path)); }

std::string getRootDirWithUtf8(std::string const & utf8_path)
{
    if (utf8_path.size() < 1 || utf8_path[0] != PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return std::string() + PATH_SEPARATOR_OF_POSIX;
}

bool isAbsoluteWithUtf8(std::string const & utf8_path)
{ return !getRootDirWithUtf8(utf8_path).empty(); }

bool isRelativeWithUtf8(std::string const & utf8_path)
{ return !isAbsoluteWithUtf8(utf8_path); }

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{ return details::removeLastNodeWithUtf8(utf8_path, unix::isPathSeparatorChar<UChar>); }

} // namespace unix
// @formatter:on

} // namespace details
// ===================

static int CREATE_TEMPDIR_RETRY_COUNT = 10;

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
        string::createRandomString(&buffer[0], buffer.size());
        cursor = prefix + buffer + suffix;

        if (isDirectory(cursor) == false && createDirectory(cursor)) {
            return cursor;
        }
    }

    return std::string();
}

std::string createDefaultTempDir()
{
    std::string temp_dir = getTempDir();
    if (isDirectory(temp_dir) == false) {
        return std::string();
    }

    if (temp_dir[temp_dir.size() - 1] != PATH_SEPARATOR) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

bool removeAll(std::string const & path)
{
    if (isDirectory(path)) {
        bool result = true;
        for (auto & cursor : scanDir(path)) {
            if (cursor != "." && cursor != "..") {
                result &= removeAll(path + PATH_SEPARATOR + cursor);
            }
        }
        result &= removeDirectory(path);
        return result;
    }
    return removeFile(path);
}

void printInfos(std::ostream * stream)
{
    if (stream == nullptr) {
        stream = &std::cout;
    }

    (*stream) << "Filesystem information:\n";
    if (isWindowsPlatform()) {
        (*stream) << " * Windows Platform Filesystem.\n";
    }
    if (isUnixLikePlatform()) {
        (*stream) << " * Unix-like Platform Filesystem.\n";
    }

    (*stream) << " * Temp directory: " << getTempDir() << std::endl;
    (*stream) << " * Home directory: " << getHomeDir() << std::endl;
    (*stream) << " * Work directory: " << getWorkDir() << std::endl;
    (*stream) << " * Work directory (realpath): " << getRealPath(".") << std::endl;
    (*stream) << " * Exe path: "       << getExePath() << std::endl;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

