/**
 * @file   FsWinUtils.cpp
 * @brief  FsWinUtils class implementation.
 * @author zer0
 * @date   2017-03-20
 */

#include <libtbag/filesystem/details/windows/FsWinUtils.hpp>
#include <libtbag/log/Log.hpp>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
# include <Shlobj.h>  // SHGetFolderPath
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

std::string getTempDir()
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    // The maximum possible return value is MAX_PATH+1 (261).
    DWORD const BUFFER_LENGTH  = MAX_PATH + 1;
    char buffer[BUFFER_LENGTH] = {0,};

    DWORD const COPIED_LENGTH = GetTempPathA(BUFFER_LENGTH, buffer);
    // COPIED_LENGTH is not including the terminating null character.

    if (COPIED_LENGTH == 0) {
        __tbag_error("GetTempPathA() ERROR: {}", GetLastError());
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
        __tbag_error("GetCurrentDirectoryA() FIRST ERROR: {}", GetLastError());
        return std::string();

    } else if (WRITTEN_LENGTH > BUFFER_LENGTH) {
        // If the buffer that is pointed to by lpBuffer is not large enough,
        // the return value specifies the required size of the buffer,
        // in characters, including the null-terminating character.
        std::string result;
        result.resize(WRITTEN_LENGTH);
        result[WRITTEN_LENGTH - 1] = '\0';

        if (GetCurrentDirectoryA(WRITTEN_LENGTH, &result[0]) == 0) {
            __tbag_error("GetCurrentDirectoryA() SECOND ERROR: {}", GetLastError());
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
        __tbag_error("SHGetFolderPathA() ERROR: {}", RESULT);
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
        __tbag_error("GetModuleFileNameA() ERROR: {}", GetLastError());
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

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

