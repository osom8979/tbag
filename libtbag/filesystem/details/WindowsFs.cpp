/**
 * @file   WindowsFs.cpp
 * @brief  Windows Filesystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: WindowsFileSystem -> WindowsFs)
 */

#include <libtbag/filesystem/details/WindowsFs.hpp>
#include <libtbag/filesystem/details/FsTemplate.hpp-inl>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsCheck.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cassert>

#include <unicode/unistr.h>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
# include <Shlobj.h>  // SHGetFolderPath
# include <Shlwapi.h> // PathFileExists
# include <Strsafe.h> // StringCchLength, etc ...
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif
#include <libtbag/proxy/windows/String.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

#ifndef __ASSERT_NOT_IMPLEMENT
#define __ASSERT_NOT_IMPLEMENT(retval) \
    do { if (isWindowsPlatform() == false) { assert(0 && "Not implement."); return retval; } } while(0)
#endif

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
    std::string temp_dir = details::getTempDir();
    if (isDirectory(temp_dir) == false) {
        return std::string();
    }

    char const LAST_CHAR = temp_dir[temp_dir.size() - 1];
    if (LAST_CHAR != PATH_SEPARATOR_OF_WINDOWS || LAST_CHAR != PATH_SEPARATOR_OF_POSIX) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR_OF_WINDOWS;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

std::string getRealPath(std::string const & path)
{
    __ASSERT_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);

    if (WCS_PATH.empty()) {
        return std::string();
    }

    wchar_t ** part = {nullptr};
    DWORD const RESERVE_SIZE = GetFullPathNameW(&WCS_PATH[0], 0, nullptr, part);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetFullPathNameW(&WCS_PATH[0], buffer.size(), &buffer[0], part);
    if (COPIED_LENGTH == 0) {
        __tbag_error("GetFullPathNameW() ERROR: {}", GetLastError());
    }

    buffer.resize(COPIED_LENGTH);
    return proxy::windows::wcsToMbsWithAcp(buffer);
}

bool createDirectory(std::string const & path)
{
    __ASSERT_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (CreateDirectoryW(&WCS_PATH[0], nullptr) == FALSE) {
        // ERROR_ALREADY_EXISTS: // The specified directory already exists.
        // ERROR_PATH_NOT_FOUND: // One or more intermediate directories do not exist.
        __tbag_error("CreateDirectoryW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeDirectory(std::string const & path)
{
    __ASSERT_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (RemoveDirectoryW(&WCS_PATH[0]) == FALSE) {
        __tbag_error("RemoveDirectoryW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeFile(std::string const & path)
{
    __ASSERT_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (DeleteFileW(&WCS_PATH[0]) == FALSE) {
        // ERROR_FILE_NOT_FOUND: // The system cannot find the file specified.
        // ERROR_ACCESS_DENIED:  // Access is denied.
        __tbag_error("DeleteFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeAll(std::string const & path)
{
    if (isDirectory(path)) {
        bool result = true;
        for (auto & cursor : scanDir(path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(path + PATH_SEPARATOR_OF_WINDOWS + cursor);
            }
        }
        result &= removeDirectory(path);
        return result;
    }
    return removeFile(path);
}

bool rename(std::string const & from, std::string const & to)
{
    __ASSERT_NOT_IMPLEMENT(false);

    std::wstring const WCS_FROM = proxy::windows::mbsToWcsWithAcp(from);
    std::wstring const WCS_TO   = proxy::windows::mbsToWcsWithAcp(to);

    if (WCS_FROM.empty() || WCS_TO.empty()) {
        return false;
    }

    if (MoveFileW(&WCS_FROM[0], &WCS_TO[0]) == FALSE) {
        __tbag_error("MoveFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

static std::string getLongPathName(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);
    if (WCS_PATH.empty()) {
        return std::string();
    }

    DWORD const RESERVE_SIZE = GetLongPathNameW(&WCS_PATH[0], nullptr, 0);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetLongPathNameW(&WCS_PATH[0], &buffer[0], RESERVE_SIZE);
    if (COPIED_LENGTH == 0) {
        __tbag_error("GetLongPathNameW() ERROR: {}", GetLastError());
    }
    buffer.resize(COPIED_LENGTH);
    return proxy::windows::wcsToMbsWithAcp(buffer);
}


std::vector<std::string> scanDir(std::string const & path)
{
    __ASSERT_NOT_IMPLEMENT(std::vector<std::string>());

    std::wstring const WCS_PATH = proxy::windows::mbsToWcsWithAcp(path);

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.
    std::size_t length = 0;
    if (FAILED(StringCchLengthW(&WCS_PATH[0], MAX_PATH, &length))) {
        // The value in psz is NULL, cchMax is larger than STRSAFE_MAX_CCH, or psz is longer than cchMax.
        __tbag_error("StringCchLengthW() ERROR.");
        return std::vector<std::string>();
    }

    if (length > (MAX_PATH - 3)) {
        __tbag_error("Directory path is too long.");
        return std::vector<std::string>();
    }

    wchar_t scan_directory[MAX_PATH] = { 0, };
    if (FAILED(StringCchCopyW(scan_directory, MAX_PATH, &WCS_PATH[0]))) {
        __tbag_error("StringCchCopyW ERROR.");
        return std::vector<std::string>();
    }
    if (FAILED(StringCchCatW(scan_directory, MAX_PATH, L"\\*"))) {
        __tbag_error("StringCchCatW ERROR.");
        return std::vector<std::string>();
    }

    WIN32_FIND_DATAW find_data = {0,};
    HANDLE find_handle = FindFirstFileW(scan_directory, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        __tbag_error("FindFirstFileW ERROR: {}", GetLastError());
        return std::vector<std::string>();
    }

    std::vector<std::string> result;

    // List all the files in the directory with some info about them.
    do {
        // DIRECTORY:
        //  @code
        //    find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
        //  @endcode
        // SIZE:
        //  @code
        //    LARGE_INTEGER file_size
        //    file_size.LowPart = find_data.nFileSizeLow;
        //    file_size.HighPart = find_data.nFileSizeHigh;
        //    static_cast<int64_t>(file_size.QuadPart);
        //  @endcode
        if (StrCmpW(L".", find_data.cFileName) != 0 && StrCmpW(L"..", find_data.cFileName) != 0) {
            result.push_back(proxy::windows::wcsToMbsWithAcp(find_data.cFileName));
        }
    } while (FindNextFileW(find_handle, &find_data) == TRUE);

    FindClose(find_handle);
    return result;
}

// --------------------------
// Filesystem path operators.
// --------------------------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return details::isProhibitedNameWithUtf8(utf8_path, windows::isProhibitedChar<UChar>);
}

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{
    return details::removeLastSeparatorWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_WINDOWS, windows::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, windows::isPathSeparatorChar<UChar>);
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

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{
    return details::removeLastNodeWithUtf8(utf8_path, windows::isPathSeparatorChar<UChar>);
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

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

