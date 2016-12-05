/**
 * @file   WindowsFileSystem.cpp
 * @brief  WindowsFileSystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 */

#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/log/Log.hpp>

#if defined(__PLATFORM_WINDOWS__)
#include <Windows.h>
#include <Shlobj.h>  // SHGetFolderPath
#include <Shlwapi.h> // PathFileExists
#include <Strsafe.h> // StringCchLength
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

#if defined(__PLATFORM_WINDOWS__)
/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd319072(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Stringapiset.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
static std::wstring mbsToWcs(std::string const & path)
{
    if (path.empty()) {
        __tbag_error("Illegal argument: path is 0 length.");
        return std::wstring();
    }

    int const WRITTEN_LENGTH = MultiByteToWideChar(CP_ACP, 0, &path[0], path.size(), NULL, 0);
    std::wstring result;

    if (WRITTEN_LENGTH == 0) {
        result.resize(path.size());
    } else {
        result.resize(WRITTEN_LENGTH + 1);
    }

    int const WRITTEN_LENGTH2 = MultiByteToWideChar(CP_ACP, 0, &path[0], path.size(), &result[0], result.size());
    if (WRITTEN_LENGTH2 == 0) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_INSUFFICIENT_BUFFER:     // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        case ERROR_INVALID_FLAGS:           // The values supplied for flags were not valid.
        case ERROR_INVALID_PARAMETER:       // Any of the parameter values was invalid.
        case ERROR_NO_UNICODE_TRANSLATION:  // Invalid Unicode was found in a string.
        default:
            __tbag_error_f("MultiByteToWideChar() ERROR: {}", ERROR_CODE);
        }
        return std::wstring();
    }

    result.resize(WRITTEN_LENGTH2);
    return result;
}

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd374130(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Stringapiset.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
static std::string wcsToMbs(std::wstring const & path)
{
    if (path.empty()) {
        __tbag_error("Illegal argument: path is 0 length.");
        return std::string();
    }

    int const WRITTEN_LENGTH = WideCharToMultiByte(CP_ACP, 0, &path[0], path.size(), NULL, 0, NULL, NULL);
    std::string result;

    if (WRITTEN_LENGTH == 0) {
        result.resize(path.size());
    } else {
        result.resize(WRITTEN_LENGTH + 1);
    }

    int const WRITTEN_LENGTH2 = WideCharToMultiByte(CP_ACP, 0, &path[0], path.size(), &result[0], result.size(), NULL, NULL);
    if (WRITTEN_LENGTH2 == 0) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_INSUFFICIENT_BUFFER:     // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        case ERROR_INVALID_FLAGS:           // The values supplied for flags were not valid.
        case ERROR_INVALID_PARAMETER:       // Any of the parameter values was invalid.
        case ERROR_NO_UNICODE_TRANSLATION:  // Invalid Unicode was found in a string.
        default:
            __tbag_error_f("WideCharToMultiByte() ERROR: {}", ERROR_CODE);
        }
        return std::string();
    }

    result.resize(WRITTEN_LENGTH2);
    return result;
}

static DWORD getAttribute(std::string const & path)
{
    std::wstring const WCS_PATH = mbsToWcs(path);
    if (WCS_PATH.size() == 0) {
        return GetFileAttributesA(&path[0]);
    } else {
        return GetFileAttributesW(&WCS_PATH[0]);
    }
}

static bool checkPermission(std::string const & path, DWORD permission)
{
    SECURITY_INFORMATION const SECURITY = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
    std::wstring const WCS_PATH = mbsToWcs(path);

    bool bRet = false;
    DWORD length = 0;

    if (!::GetFileSecurityW(&WCS_PATH[0], OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION
        | DACL_SECURITY_INFORMATION, NULL, NULL, &length) && ERROR_INSUFFICIENT_BUFFER == ::GetLastError()) {

        PSECURITY_DESCRIPTOR security = static_cast< PSECURITY_DESCRIPTOR >(::malloc(length));
        if (security && ::GetFileSecurityW(&WCS_PATH[0], OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION
            | DACL_SECURITY_INFORMATION, security, length, &length)) {
            HANDLE hToken = NULL;
            if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_IMPERSONATE | TOKEN_QUERY |
                TOKEN_DUPLICATE | STANDARD_RIGHTS_READ, &hToken)) {
                HANDLE hImpersonatedToken = NULL;
                if (::DuplicateToken(hToken, SecurityImpersonation, &hImpersonatedToken)) {
                    GENERIC_MAPPING mapping = { 0xFFFFFFFF };
                    PRIVILEGE_SET privileges = { 0 };
                    DWORD grantedAccess = 0, privilegesLength = sizeof(privileges);
                    BOOL result = FALSE;

                    mapping.GenericRead = FILE_GENERIC_READ;
                    mapping.GenericWrite = FILE_GENERIC_WRITE;
                    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
                    mapping.GenericAll = FILE_ALL_ACCESS;

                    ::MapGenericMask(&permission, &mapping);
                    if (::AccessCheck(security, hImpersonatedToken, permission,
                        &mapping, &privileges, &privilegesLength, &grantedAccess, &result)) {
                        bRet = (result == TRUE);
                    }
                    ::CloseHandle(hImpersonatedToken);
                }
                ::CloseHandle(hToken);
            }
            ::free(security);
        }
    }

    return bRet;
}
#endif

std::string getTempDir()
{
#if defined(__PLATFORM_WINDOWS__)
    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        __tbag_error_f("GetTempPathA() ERROR: {}", GetLastError());
        return std::string();
    }
    return std::string(buffer);
#else
    return std::string();
#endif
}

std::string getWorkDir()
{
#if defined(__PLATFORM_WINDOWS__)
    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    // Multithreaded applications and shared library code should not use the GetCurrentDirectory function
    // and should avoid using relative path names.
    DWORD const WRITTEN_LENGTH = GetCurrentDirectoryA(BUFFER_LENGTH, buffer);

    if (WRITTEN_LENGTH == 0) {
        __tbag_error_f("GetCurrentDirectoryA() FIRST ERROR: {}", GetLastError());
        return std::string();

    } else if (WRITTEN_LENGTH > BUFFER_LENGTH) {
        // If the buffer that is pointed to by lpBuffer is not large enough,
        // the return value specifies the required size of the buffer,
        // in characters, including the null-terminating character.
        std::string result;
        result.resize(WRITTEN_LENGTH);
        result[WRITTEN_LENGTH - 1] = '\0';

        if (GetCurrentDirectoryA(WRITTEN_LENGTH, &result[0]) == 0) {
            __tbag_error_f("GetCurrentDirectoryA() SECOND ERROR: {}", GetLastError());
            return std::string();
        }
        return result;
    }

    return std::string(buffer);
#else
    return std::string();
#endif
}

std::string getHomeDir()
{
#if defined(__PLATFORM_WINDOWS__)
    // See more: Access Control.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa374860.aspx
    //HANDLE const DEFAULT_USER_TOKEN = (HANDLE)-1;

    HANDLE const USUALLY_TOKEN = NULL;

    DWORD const BUFFER_LENGTH = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = { 0, };

    HRESULT const RESULT = SHGetFolderPathA(NULL, CSIDL_PROFILE, USUALLY_TOKEN, 0, buffer);
    if (RESULT != S_OK) {
        __tbag_error_f("SHGetFolderPathA() ERROR: {}", RESULT);
        return std::string();
    }
    return std::string(buffer);
#else
    return std::string();
#endif
}

std::string getExePath(std::size_t extend_buffer_size)
{
#if defined(__PLATFORM_WINDOWS__)
    std::string buffer(extend_buffer_size, '\0');

    // hModule: If this parameter is NULL, GetModuleFileName retrieves the path of the executable file of the current process.
    DWORD const COPIED_LENGTH = GetModuleFileNameA(NULL, &buffer[0], buffer.size());

    if (COPIED_LENGTH == 0) {
        __tbag_error_f("GetModuleFileNameA() ERROR: {}", GetLastError());
        return std::string();

    } else if (COPIED_LENGTH == buffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        // If the buffer is too small to hold the module name,
        // the string is truncated to nSize characters including the terminating null character,
        // the function returns nSize, and the function sets the last error to ERROR_INSUFFICIENT_BUFFER.
        return getExePath(extend_buffer_size * 2);
    }

    // If the function succeeds,
    // the return value is the length of the string that is copied to the buffer,
    // in characters, not including the terminating null character.
    buffer.resize(COPIED_LENGTH);
    return buffer;
#else
    return std::string();
#endif
}

std::string getRealPath(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return std::string();
#else
    return std::string();
#endif
}

bool createDirectory(std::string const & path, int mode)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = CreateDirectoryA(&path[0], NULL);
    } else {
        result = CreateDirectoryW(&WCS_PATH[0], NULL);
    }

    if (result == FALSE) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_ALREADY_EXISTS: // The specified directory already exists.
        case ERROR_PATH_NOT_FOUND: // One or more intermediate directories do not exist.
        default:
            __tbag_error_f("CreateDirectory() ERROR: {}", ERROR_CODE);
        }
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool removeDirectory(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = RemoveDirectoryA(&path[0]);
    } else {
        result = RemoveDirectoryW(&WCS_PATH[0]);
    }

    if (result == FALSE) {
        __tbag_error_f("RemoveDirectory() ERROR: {}", GetLastError());
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool removeFile(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_PATH = mbsToWcs(path);
    BOOL result = FALSE;

    if (WCS_PATH.size() == 0) {
        result = DeleteFileA(&path[0]);
    } else {
        result = DeleteFileW(&WCS_PATH[0]);
    }

    if (result == FALSE) {
        DWORD const ERROR_CODE = GetLastError();
        switch (ERROR_CODE) {
        case ERROR_FILE_NOT_FOUND:  // The system cannot find the file specified.
        case ERROR_ACCESS_DENIED:   // Access is denied.
        default:
            __tbag_error_f("DeleteFile() ERROR: {}", ERROR_CODE);
        }
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool rename(std::string const & from, std::string const & to)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_FROM = mbsToWcs(from);
    std::wstring const WCS_TO   = mbsToWcs(to);
    BOOL result = FALSE;

    if (WCS_FROM.size() == 0 || WCS_TO.size() == 0) {
        result = MoveFileA(&from[0], &to[0]);
    } else {
        result = MoveFileW(&WCS_FROM[0], &WCS_TO[0]);
    }

    if (result == FALSE) {
        __tbag_error_f("MoveFile() ERROR: {}", GetLastError());
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool exists(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_PATH = mbsToWcs(path);
    if (WCS_PATH.size() == 0) {
        return (PathFileExistsA(&path[0]) == TRUE);
    } else {
        return (PathFileExistsW(&WCS_PATH[0]) == TRUE);
    }
#else
    return false;
#endif
}

bool isDirectory(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    DWORD const ATTRIBUTES = getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#else
    return false;
#endif
}

bool isRegularFile(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    DWORD const ATTRIBUTES = getAttribute(path);
    return (ATTRIBUTES != INVALID_FILE_ATTRIBUTES && (ATTRIBUTES & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
#else
    return false;
#endif
}

bool isExecutable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return checkPermission(path, GENERIC_EXECUTE);
#else
    return false;
#endif
}

bool isWritable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return checkPermission(path, GENERIC_WRITE);
#else
    return false;
#endif
}

bool isReadable(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    return checkPermission(path, GENERIC_READ);
#else
    return false;
#endif
}

std::vector<std::string> scanDir(std::string const & path)
{
#if defined(__PLATFORM_WINDOWS__)
    std::wstring const WCS_PATH = mbsToWcs(path);

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

    WIN32_FIND_DATAW find_data = { 0, };
    HANDLE find_handle = FindFirstFileW(scan_directory, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        __tbag_error_f("FindFirstFileW ERROR: {}", GetLastError());
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
            result.push_back(wcsToMbs(find_data.cFileName));
        }
    } while (FindNextFileW(find_handle, &find_data) == TRUE);

    FindClose(find_handle);
    return result;
#else
    return std::vector<std::string>();
#endif
}

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

