/**
 * @file   FsWinCommon.cpp
 * @brief  FsWinCommon class implementation.
 * @author zer0
 * @date   2017-03-21
 */

#include <libtbag/filesystem/details/windows/FsWinCommon.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <Windows.h>
# include <Shlwapi.h> // StrCmp, PathFileExists
# include <Shlobj.h>  // SHGetFolderPath
# include <Strsafe.h> // StringCchLength, etc ...
#else
# include <libtbag/dummy/Win32.hpp>
using namespace ::libtbag::dummy::win32;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

// ---------------
namespace __impl {
// ---------------

static DWORD getAttribute(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(INVALID_FILE_ATTRIBUTES);
    return GetFileAttributesW(&locale::windows::mbsToWcs(path)[0]);
}

/**
 * PSECURITY_DESCRIPTOR utility class.
 */
struct SecurityDescriptor
{
    PSECURITY_DESCRIPTOR sd;

    SecurityDescriptor(std::size_t size) : sd(static_cast<PSECURITY_DESCRIPTOR>(::malloc(size)))
    { /* EMPTY. */ }
    ~SecurityDescriptor()
    { ::free(sd); }

    PSECURITY_DESCRIPTOR get()
    { return sd; }

    inline operator bool()
    { return sd != nullptr; }
};

static bool checkPermission(std::string const & path, DWORD permission)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    SECURITY_INFORMATION const SECURITY       = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
    DWORD                const DESIRED_ACCESS = TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ;

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(path);

    DWORD sd_length = 0;
    GetFileSecurityW(&WCS_PATH[0], SECURITY, nullptr, 0, &sd_length);
    if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) {
        tDLogE("Not ERROR_INSUFFICIENT_BUFFER error.");
        return false;
    }

    SecurityDescriptor sd(sd_length);
    if (static_cast<bool>(sd) == false) {
        tDLogE("PSECURITY_DESCRIPTOR is nullptr.");
        return false;
    }
    if (GetFileSecurityW(&WCS_PATH[0], SECURITY, sd.get(), sd_length, &sd_length) == FALSE) {
        tDLogE("GetFileSecurityW() ERROR: {}", GetLastError());
        return false;
    }

    HANDLE ptoken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), DESIRED_ACCESS, &ptoken) == FALSE) {
        tDLogE("OpenProcessToken() ERROR: {}", GetLastError());
        return false;
    }

    HANDLE impersonated_token = NULL;
    if (DuplicateToken(ptoken, SecurityImpersonation, &impersonated_token) == FALSE) {
        tDLogE("DuplicateToken() ERROR: {}", GetLastError());
        CloseHandle(ptoken);
        return false;
    }

    GENERIC_MAPPING mapping  = {0xFFFFFFFF};
    PRIVILEGE_SET privileges = {0,};
    DWORD granted_access     = 0;
    DWORD privileges_length  = sizeof(privileges);
    BOOL  access_status      = FALSE;
    bool  result             = false;

    mapping.GenericRead    = FILE_GENERIC_READ;
    mapping.GenericWrite   = FILE_GENERIC_WRITE;
    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
    mapping.GenericAll     = FILE_ALL_ACCESS;

    MapGenericMask(&permission, &mapping);
    if (AccessCheck(sd.get(), impersonated_token, permission, &mapping, &privileges, &privileges_length, &granted_access, &access_status)) {
        result = (access_status == TRUE ? true : false);
    } else {
        tDLogE("AccessCheck() ERROR: {}", GetLastError());
    }

    CloseHandle(impersonated_token);
    CloseHandle(ptoken);

    return result;
}

// ------------------
} // namespace __impl
// ------------------

std::string getTempDir()
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH  = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = {0,};

    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        tDLogE("GetTempPathA() ERROR: {}", GetLastError());
        return std::string();
    }
    return std::string(buffer);
}

std::string getWorkDir()
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    DWORD const BUFFER_LENGTH  = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = {0,};

    // Multithreaded applications and shared library code should not use the GetCurrentDirectory function
    // and should avoid using relative path names.
    DWORD const WRITTEN_LENGTH = GetCurrentDirectoryA(BUFFER_LENGTH, buffer);

    if (WRITTEN_LENGTH == 0) {
        tDLogE("GetCurrentDirectoryA() FIRST ERROR: {}", GetLastError());
        return std::string();

    } else if (WRITTEN_LENGTH > BUFFER_LENGTH) {
        // If the buffer that is pointed to by lpBuffer is not large enough,
        // the return value specifies the required size of the buffer,
        // in characters, including the null-terminating character.
        std::string result;
        result.resize(WRITTEN_LENGTH);
        result[WRITTEN_LENGTH - 1] = '\0';

        if (GetCurrentDirectoryA(WRITTEN_LENGTH, &result[0]) == 0) {
            tDLogE("GetCurrentDirectoryA() SECOND ERROR: {}", GetLastError());
            return std::string();
        }
        return result;
    }

    return std::string(buffer);
}

std::string getHomeDir()
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    // See more: Access Control.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa374860.aspx
    //HANDLE const DEFAULT_USER_TOKEN = (HANDLE)-1;

    HANDLE const USUALLY_TOKEN = NULL;

    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = {0,};

    HRESULT const RESULT = SHGetFolderPathA(NULL, CSIDL_PROFILE, USUALLY_TOKEN, 0, buffer);
    if (RESULT != S_OK) {
        tDLogE("SHGetFolderPathA() ERROR: {}", RESULT);
        return std::string();
    }
    return std::string(buffer);
}

std::string getExePathEx(std::size_t extend_buffer_size)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::string buffer(extend_buffer_size, '\0');

    // hModule: If this parameter is NULL, GetModuleFileName retrieves the path of the executable file of the current process.
    DWORD const COPIED_LENGTH = GetModuleFileNameA(NULL, &buffer[0], buffer.size());

    if (COPIED_LENGTH == 0) {
        tDLogE("GetModuleFileNameA() ERROR: {}", GetLastError());
        return std::string();

    } else if (COPIED_LENGTH == buffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // If the buffer is too small to hold the module name,
        // the string is truncated to nSize characters including the terminating null character,
        // the function returns nSize, and the function sets the last error to ERROR_INSUFFICIENT_BUFFER.
        return getExePathEx(extend_buffer_size * 2);
    }

    // If the function succeeds,
    // the return value is the length of the string that is copied to the buffer,
    // in characters, not including the terminating null character.
    buffer.resize(COPIED_LENGTH);
    return buffer;
}

std::string getExePath()
{
    return getExePathEx(MAX_PATH_LENGTH + 1);
}

bool createDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (CreateDirectoryW(&WCS_PATH[0], nullptr) == FALSE) {
        // ERROR_ALREADY_EXISTS: // The specified directory already exists.
        // ERROR_PATH_NOT_FOUND: // One or more intermediate directories do not exist.
        tDLogE("CreateDirectoryW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; retry < CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
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

    char const LAST_CHAR = temp_dir[temp_dir.size() - 1];
    if (LAST_CHAR != PATH_SEPARATOR_OF_WINDOWS || LAST_CHAR != PATH_SEPARATOR_OF_POSIX) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR_OF_WINDOWS;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

bool removeDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (RemoveDirectoryW(&WCS_PATH[0]) == FALSE) {
        tDLogE("RemoveDirectoryW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeFile(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (DeleteFileW(&WCS_PATH[0]) == FALSE) {
        // ERROR_FILE_NOT_FOUND: // The system cannot find the file specified.
        // ERROR_ACCESS_DENIED:  // Access is denied.
        tDLogE("DeleteFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeFileWithUtf8(std::string const & utf8_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(utf8_path, locale::windows::CODE_PAGE_UTF8);
    if (WCS_PATH.empty()) {
        return false;
    }

    if (DeleteFileW(&WCS_PATH[0]) == FALSE) {
        // ERROR_FILE_NOT_FOUND: // The system cannot find the file specified.
        // ERROR_ACCESS_DENIED:  // Access is denied.
        tDLogE("DeleteFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool removeAll(std::string const & acp_path)
{
    if (isDirectory(acp_path)) {
        bool result = true;
        for (auto & cursor : scanDir(acp_path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(acp_path + PATH_SEPARATOR_OF_WINDOWS + cursor);
            }
        }
        result &= removeDirectory(acp_path);
        return result;
    }
    return removeFile(acp_path);
}

bool rename(std::string const & acp_path_from, std::string const & acp_path_to)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);

    std::wstring const WCS_FROM = locale::windows::mbsToWcs(acp_path_from);
    std::wstring const WCS_TO   = locale::windows::mbsToWcs(acp_path_to);

    if (WCS_FROM.empty() || WCS_TO.empty()) {
        return false;
    }

    if (MoveFileW(&WCS_FROM[0], &WCS_TO[0]) == FALSE) {
        tDLogE("MoveFileW() ERROR: {}", GetLastError());
        return false;
    }
    return true;
}

bool exists(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return (PathFileExistsW(&locale::windows::mbsToWcs(acp_path)[0]) == TRUE);
}

bool isDirectory(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(acp_path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

bool isRegularFile(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    DWORD const ATTRIBUTES = __impl::getAttribute(acp_path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

bool isExecutable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_EXECUTE);
}

bool isWritable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_WRITE);
}

bool isReadable(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(false);
    return __impl::checkPermission(acp_path, GENERIC_READ);
}

std::string getRealPath(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);

    if (WCS_PATH.empty()) {
        return std::string();
    }

    wchar_t ** part = {nullptr};
    DWORD const RESERVE_SIZE = GetFullPathNameW(&WCS_PATH[0], 0, nullptr, part);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetFullPathNameW(&WCS_PATH[0], buffer.size(), &buffer[0], part);
    if (COPIED_LENGTH == 0) {
        tDLogE("GetFullPathNameW() ERROR: {}", GetLastError());
    }

    buffer.resize(COPIED_LENGTH);
    return locale::windows::wcsToMbs(buffer);
}

std::string getLongPathName(std::string const & acp_path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(acp_path);
    if (WCS_PATH.empty()) {
        return std::string();
    }

    DWORD const RESERVE_SIZE = GetLongPathNameW(&WCS_PATH[0], nullptr, 0);
    std::wstring buffer;
    buffer.resize(RESERVE_SIZE);

    DWORD const COPIED_LENGTH = GetLongPathNameW(&WCS_PATH[0], &buffer[0], RESERVE_SIZE);
    if (COPIED_LENGTH == 0) {
        tDLogE("GetLongPathNameW() ERROR: {}", GetLastError());
    }
    buffer.resize(COPIED_LENGTH);
    return locale::windows::wcsToMbs(buffer);
}

std::vector<std::string> scanDir(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::vector<std::string>());

    std::wstring const WCS_PATH = locale::windows::mbsToWcs(path);

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.
    std::size_t length = 0;
    if (FAILED(StringCchLengthW(&WCS_PATH[0], MAX_PATH, &length))) {
        // The value in psz is NULL, cchMax is larger than STRSAFE_MAX_CCH, or psz is longer than cchMax.
        tDLogE("StringCchLengthW() ERROR.");
        return std::vector<std::string>();
    }

    if (length > (MAX_PATH - 3)) {
        tDLogE("Directory path is too long.");
        return std::vector<std::string>();
    }

    wchar_t scan_directory[MAX_PATH] = { 0, };
    if (FAILED(StringCchCopyW(scan_directory, MAX_PATH, &WCS_PATH[0]))) {
        tDLogE("StringCchCopyW ERROR.");
        return std::vector<std::string>();
    }
    if (FAILED(StringCchCatW(scan_directory, MAX_PATH, L"\\*"))) {
        tDLogE("StringCchCatW ERROR.");
        return std::vector<std::string>();
    }

    WIN32_FIND_DATAW find_data = {0,};
    HANDLE find_handle = FindFirstFileW(scan_directory, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        tDLogE("FindFirstFileW ERROR: {}", GetLastError());
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
            result.push_back(locale::windows::wcsToMbs(find_data.cFileName));
        }
    } while (FindNextFileW(find_handle, &find_data) == TRUE);

    FindClose(find_handle);
    return result;
}


} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

